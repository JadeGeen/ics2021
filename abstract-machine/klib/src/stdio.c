#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

char *itoa(int value, char *string, int radix){
	int i;
	char temp[100]={0};
	for(i=0;value>0;i++){
		temp[i]=value%radix;
		value=value/radix;
	}
	for(int j=i-1;j>=0;j++){
		*string=temp[j];
		string++;
	}
	return string;//directly return last point
}

int printf(const char *fmt, ...) {
	panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	char *str;
	char *s;
	int len;
	for(str=out;*fmt;fmt++){
		if(*fmt!='%'){
			*str=*fmt;
			fmt++;
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
		}
	}
	*str='\0';
	int ret=str-out;
	return ret;
}

int sprintf(char *out, const char *fmt, ...) {
	va_list args;
	int i;
	va_start(args, fmt);
	i=vsprintf(out, fmt, args);
	va_end(args);
	return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
	panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
	panic("Not implemented");
}

#endif
