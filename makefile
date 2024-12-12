#--컴파일러 변수----
CXX = g++

#--플래그 및 라이브러리 설정----
OPENCV = `pkg-config --cflags --libs opencv4`
OPENSSL_LIBS = -lssl -lcrypto
MYSQL_LIBS = -lmysqlclient
THREAD_LIBS = -lpthread
CXXFLAGS = -Wall -Wextra -g -O1

#--폴더 설정----
DATABASE_DIR = Database
HASH_DIR = Hash
VERIFY_DIR = Verify

#--소스 파일 및 오브젝트 파일 설정----
SRC = verifier.cpp \
      $(DATABASE_DIR)/database.cpp \
      $(HASH_DIR)/hash.cpp \
      $(VERIFY_DIR)/verify.cpp

OBJ = verifier.o \
      $(DATABASE_DIR)/database.o \
      $(HASH_DIR)/hash.o \
      $(VERIFY_DIR)/verify.o

#--기본 타겟----
all: verifier

#--타겟 파일을 생성----
verifier: $(OBJ)
	$(CXX) $(OBJ) -o $@ $(OPENCV) $(OPENSSL_LIBS) $(MYSQL_LIBS) $(THREAD_LIBS) $(CXXFLAGS)

#--개별 소스 파일을 오브젝트 파일로 컴파일하는 규칙----
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS) $(OPENCV)

#--clean 규칙----
clean:
	rm -f $(OBJ) verifier
