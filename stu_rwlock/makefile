.PHONY:all
all:stu_rwlock
stu_rwlock:stu_rwlock.c
	g++ -std=c++11 -lpthread $^ -o $@
.PHONY:clean
clean:
	rm -rf stu_rwlock
