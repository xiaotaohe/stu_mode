.PHONY:all
all:stu_sig stu_sig0 stu_sig1
stu_sig1:stu_sig1.c
	gcc $^ -o $@
stu_sig:stu_sig.c
	gcc $^ -o $@
stu_sig0:stu_sig0.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm -rf stu_sig stu_sig0
