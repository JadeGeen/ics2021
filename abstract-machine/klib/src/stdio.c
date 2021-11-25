#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char *itoa(int value, char *string, int radix){
	int i;
	char temp[100]={0};
	for(i=0;value>0;i++){
		temp[i]=value%radix+'0';
		value=value/radix;
	}
	for(int j=i-1;j>=0;j--){
		*string=temp[j];
		string++;
	}
	return string;//directly return last point
}


int vsprintf(char *out, const char *fmt, va_list ap) {
	char *str;
	char *s;
	int len;
	char c;
	for(str=out;*fmt;fmt++){
		if(*fmt!='%'){
			*str=*fmt;
			str++;
			continue;
		}
		fmt++;//skip the first'%'
		switch(*fmt){
			case 's':
				s=va_arg(ap, char*);
				len=strlen(s);
				for(int i=0;i<len;i++){
					*str=*s;
					str++;
					s++;
				}
				break;
			case 'd':
				str=itoa(va_arg(ap, int),str,10);
				break;
			case 'c':
				c=va_arg(ap,int);
				*str=c;
				str++;
				break;
		}
	}
	*str='\0';
	return (str-out);
}

int sprintf(char *out, const char *fmt, ...) {
	va_list args;
	int i;
	va_start(args, fmt);
	i=vsprintf(out, fmt, args);
	va_end(args);
	return i;
}

int printf(const char *fmt, ...){
	va_list args;
	int i;
	char buf[2048]={'\0'};
	va_start(args,fmt);
	i=vsprintf(buf, fmt, args);
	va_end(args);

	int j=0;
	while(buf[j]!='\0')
		putch(buf[j]);
	return i;
}
int snprintf(char *out, size_t n, const char *fmt, ...) {
	panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
	panic("Not implemented");
}

#endif
