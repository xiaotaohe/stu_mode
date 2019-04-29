#生成静态库、动态库
.PHONY:all
all:print print0 hello1 hello2
#生成静态库
print:print.c
	gcc -c print.c -o print.o
	ar -rc libprint.a print.o
#生成动态库
print0:print.c
	gcc -fPIC -c print.c
	gcc -shared -o libprint.so print.o
#编译hello
#静态库编译
hello1:hello.c
	gcc hello.c libprint.a -o hello1 
#动态库编译
hello2:hello.c
	gcc hello.c -L. libprint.so -o hello2
.PHONY:clean
clean:
	rm -rf print print0


#动态库静态库还可以直接编译:
#如：gcc hello.c libprint -o hello //利用静态库编译hello.c
#    gcc hello.c -L. -o -lprint hello //利用动态库编译hello.c
