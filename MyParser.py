import shutil

def get_args(input_string):
    input_string = input_string.replace("\n"," ")
    function_name, data = input_string.split(" : ", 1)
    function_name = function_name.replace(" ","")
    data = data.replace("\n","").strip("{}").split(",")
    kv_pairs = []
    for pair in data:
        key, value = pair.split(" : ")
        kv_pairs.append( (key.strip().replace(" ","").strip("{}"),value.strip().replace(" ","").strip("{}")))
    return function_name, kv_pairs

def generate_make(function_name, kv_pairs, output_file):
    code = f"static void Make{function_name}(SendBuffer& buffer"
    for key, value in kv_pairs:
        code += f", const {key} {value}"
    code += ")\n{\n"
    code += f"\tbuffer << ePacketType::e{function_name}"
    for _, value in kv_pairs:
        code += f" << {value}"
    code += ";\n}\n\n"
    output_file.write(code)

def generate_get(function_name, kv_pairs, output_file):
    code = f"static void Get{function_name}(CRecvBuffer& buffer"
    for key, value in kv_pairs:
        code += f", {key}& {value}"
    code += ")\n{\n"
    code += f"\tbuffer"
    for _, value in kv_pairs:
        code += f" >> {value}"
    code += ";\n}\n\n"
    output_file.write(code)

def generate_enums(function_group,output_file):
    code = f"enum ePacketType : PacketType\n"
    code += "{\n"
    for key ,_ in function_group.items():
        code += f"e{key},\n"
    code += "None,\n"
    code += "};\n\n"
    output_file.write(code) 
    
def generate_head(output_file):
        output_file.write("#pragma once\n")
        output_file.write("#include \"ContentTypes.h\"\n")
        output_file.write("#include \"FVector.h\"\n")
        output_file.write("#include \"SendBuffer.h\"\n")
        output_file.write("#include \"CRecvBuffer.h\"\n")
        output_file.write("namespace psh \n{\n")

def generate_struct(function_name, kv_pairs,output_file):
    code = f'struct st{function_name}\n{{\n'
    for key, value in kv_pairs:
        code += f"{key} {value};\n"
    code += f'}};\n'
    output_file.write(code) 

def generate_union(function_names,output_file):
    code = f"\nunion\n{{\n"
    for key, value in function_names.items():
        code += f"        st{key} {key}Info;\n"
    code += f"}};\n"
    output_file.write(code) 

def generate_tail(output_file):
    output_file.write("\n};\n")

def generate_Packet(function_data, outFile):
    generate_head(outFile)
    generate_enums(function_data,outFile)
    for key, value in function_data.items():
        generate_make(key,value,outFile)
        generate_get(key,value,outFile)
    generate_tail(outFile)

def generate_contentJob(function_data, outFile):
    function_data = {k:v for k,v in function_data.items() if 'Res' in k }
    code = f'''#pragma once
#include <PacketGenerated.h>
#include "UnrealTypes.h"
#include "TLSPool.h"
class CRecvBuffer;
namespace psh
{{
	class MYLIB_API ContentJob
	{{
		USE_TLS_POOL(ContentJob)
	public:
		ePacketType _type;
		SessionID _id;\n\n''' 
    outFile.write(code) 
    for key, value in function_data.items():
        generate_struct(key,value,outFile)
    generate_union(function_data,outFile)
    code =f'''
		void Free();
		static int64 GetPoolAllocatedSize() {{ return _pool.AllocatedCount(); }}
		static ContentJob* Alloc() 
		{{
			const auto ret = _pool.Alloc();
			ret->_type = psh::None;
			return ret;
		}}
	private:
		ContentJob() : _type(psh::None), _id() 	{{}}
        ~ContentJob() {{}}
		static TlsPool<ContentJob, 0> _pool;
	}};
}}'''
    outFile.write(code) 

def generate_unrealHeaderFunction(function_name, kv_pairs):
    parameters = ', '.join([f'const {key}& {value}' for key, value in kv_pairs])
    code = f'\n    void {function_name}({parameters});'
    return code

def generate_unrealClientHeader(function_data, outFile):
    code = f'''
#pragma once
#include "IOCP.h"
#include "PacketQueue.h"
#include "UnrealClient.generated.h"
class UChatGameInstance;
UCLASS()
class UnrealClient : public UObject, public IOCP
{{
	GENERATED_BODY()
public:
	UnrealClient() : IOCP(false) {{		IOCP::ClientInit(2,2,50,false,false,false);
	Start();
UE_LOG(LogTemp,Display,TEXT("UnreaClientCreated %p "),this); }}
	bool Dequeue(psh::ContentJob*& job){{return _jobQueue.Dequeue(job);}}\n\n
    virtual	void BeginDestroy() override;\n'''
    function_data = {k:v for k,v in function_data.items() if 'Req' in k }
    for key, value in function_data.items():
        code += generate_unrealHeaderFunction(key,value)

    code +=f'''\n\n
	void Disconnect();\n
    	void Connect(const String& ip, const Port port)
	{{
		if(IsConnect()) return;
		auto result =GetClientSession(ip,port);
		if(result.HasValue())
		{{
			_clientSession = result.Value();
		}}

	}}
private:
	virtual void OnRecvPacket(SessionID sessionId, CRecvBuffer& buffer) override;

public:
	virtual void OnDisconnect(SessionID sessionId, int wsaErr) override;
	bool IsConnect() const {{return _clientSession != InvalidSessionID();}};
private:
	PacketQueue _jobQueue;
	SessionID _clientSession = InvalidSessionID();
}};
    '''
    outFile.write(code) 

def generate_Reqest(function_name, kv_pairs):
    parameters = ', '.join([f'const {key}& {value}' for key, value in kv_pairs])
    code = f'''\nvoid UnrealClient::{function_name}({parameters})\n{{\n'''

    paramValues = ', '.join([f'{value}' for _,value in kv_pairs])
    code +=f''' auto packet = SendBuffer::Alloc();
	psh::Make{function_name}(packet,{paramValues});
	SendPacket(_clientSession,packet);\n}}\n'''
    return code

def genetate_recvCase(function_name, kv_pairs):
    code = f'''\t\tcase psh::e{function_name}:\n\t\t\t{{
\t\t\tauto job = psh::ContentJob::Alloc();\n'''

    paramValues = ', '.join([f'info.{value}' for _,value in kv_pairs])
    code += f'''\t\t\tauto& info = job->{function_name}Info;\n\t\t\tpsh::Get{function_name}(buffer,{paramValues});\n'''
    code += f'''\t\t\tjob->_type = psh::e{function_name};\n\t\t\t_jobQueue.Enqueue(job);\n\t\t\tbreak;\n\t\t\t}}\n'''
    return code

def generate_OnRecv(function_data):
    function_data = {k:v for k,v in function_data.items() if 'Res' in k }
    code = f'''void UnrealClient::OnRecvPacket(SessionID sessionId, CRecvBuffer& buffer)
{{
    psh::ePacketType type;
	while(buffer.CanPopSize() != 0)
	{{
		buffer >> type;
		switch (type)
		{{\n'''
    for key, value in function_data.items():
        code += genetate_recvCase(key,value)
    code += f'''\t\t\tdefault:\n\t\t\tASSERT_CRASH(false,"InvalidPacketHeadre");\n}}\n}}\n}}'''
    return code


def generate_unrealClientCPP(function_data, outFile):
    code = f'''#include "UnrealClient.h"\n#include "ChatGameInstance.h"\n#include "PacketGenerated.h"\n'''

    req_functions = {k:v for k,v in function_data.items() if 'Req' in k }
    for key, value in req_functions.items():
        code += generate_Reqest(key,value)
    code += generate_OnRecv(function_data)
    code += f'''void UnrealClient::OnDisconnect(SessionID sessionId, int wsaErr)
{{
	UE_LOG(LogTemp,Display,TEXT("Disconnect!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));
}}\n
void UnrealClient::BeginDestroy()
{{
	Disconnect();
	UObject::BeginDestroy();
}}\n
void UnrealClient::Disconnect()
{{
	if(!IsConnect()) return;
	UE_LOG(LogTemp,Display,TEXT("UeClientDisconnect , %d"),_clientSession.id);
	DisconnectSession(_clientSession);
	_clientSession = InvalidSessionID();
}}

'''
    outFile.write(code) 

def process_file(file_path):
    function_dictionary = dict()
    with open("PacketGenerated.h",'w') as outFile:
        with open(file_path, 'r') as packet_definitions:
            buffer = ""
            for definition in packet_definitions:
                if not definition.strip().startswith('//'):
                    buffer += definition
                    if '}' in buffer:
                        function_name, function_args = get_args(buffer)
                        function_dictionary[function_name] = function_args
                        buffer = ""
            generate_Packet(function_dictionary,outFile)
    with open("ContentJob.h","w") as jobFile:
        generate_contentJob(function_dictionary,jobFile)
    with open("UnrealClient.h","w") as outFile:
        generate_unrealClientHeader(function_dictionary,outFile)
    with open("UnrealClient.cpp","w") as outFile:
        generate_unrealClientCPP(function_dictionary,outFile)
    

src = r"."
dst = r"C:\Users\pshpj\OneDrive\university\portfolio\ChatTest\Source\ChatTest"
process_file("PacketDefine.txt")
shutil.move(src + r"\UnrealClient.h",dst+ r"\UnrealClient.h")
shutil.move(src + r"\UnrealClient.cpp",dst+ r"\UnrealClient.cpp")
shutil.move(src + r"\ContentJob.h",dst +r"\ContentJob.h")