import random

# 함수를 사용하여 SQL 쿼리 생성 및 파일에 저장
def generate_and_save_sql_queries(num_queries, filename):
    with open(filename, 'a') as file:
        file.write('set SQL_MODE="NO_AUTO_VALUE_ON_ZERO";\n')
        for i in range(0,num_queries+1):
            # 무작위 값 생성
            charType = random.randint(0, 3)
            serverType = random.randint(1, 2)
            locationX = random.randint(100, 6000)
            locationY = random.randint(100, 6000)
            
            # 계정 쿼리 생성
            account_query = f"insert into `mydb`.`account` values ({i},'ID_{i}','PASS_{i}',0);\n"
            
            # 플레이어 쿼리 생성
            player_query = f"insert into `mydb`.`player` values ({i},0,'NICK_{i}',100,0,{charType},{serverType},{locationX},{locationY});\n"
            
            # 쿼리를 파일에 쓰기
            file.write(account_query)
            file.write(player_query)

# 파일 이름 정의
filename = 'sql_queries.txt'
with open(filename, 'w') as file:
    file.write("use `mydb`;\n")
# 1000개의 SQL 쿼리 생성 및 파일에 저장
generate_and_save_sql_queries(10000, filename)

# 성공 메시지 출력
print(f"10000개의 SQL 쿼리가 {filename} 파일에 성공적으로 저장되었습니다.")

    