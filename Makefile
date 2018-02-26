PROJECT = work_good

a.out: lex.yy.c ${PROJECT}.tab.h ${PROJECT}.tab.c
		gcc -ggdb -std=gnu99 ${PROJECT}.tab.c lex.yy.c -ll -ly -lm


${PROJECT}.tab.h: ${PROJECT}.y declaration.c
		bison --report=state ${PROJECT}.y

${PROJECT}.tab.c: ${PROJECT}.y declaration.c
		bison --report=state ${PROJECT}.y

lex.yy.c: ${PROJECT}.l
	lex ${PROJECT}.l
