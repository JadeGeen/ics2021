#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s)
{
	if (s == NULL)
		return 0;
	size_t length = 0;
	for(length=0;s[length]!='\0';length++)
	;
	
	return length;
}

char *strcpy(char *dst, const char *src)
{
	if(dst==NULL)
		return NULL;
	if(src==NULL)
		return dst;
	size_t i;
	for(i=0;src[i]!='\0';i++)
		dst[i]=src[i];
	dst[i]='\0';
	return dst;
}

char *strncpy(char *dst, const char *src, size_t n)
{
	if(dst==NULL)
		return NULL;
	if(src==NULL)
		return dst;
	size_t i;
	for(i=0;i<n&&src[i]!='\0';i++)
		dst[i]=src[i];
	for(;i<=n;i++)
		dst[i]='\0';
	return dst;
}

char *strcat(char *dst, const char *src)
{
	if (dst == NULL)
		return NULL;
	if (src == NULL)
		return dst;
	size_t dst_len=strlen(dst);
	size_t i;
	for(i=0;src[i]!='\0';i++)
		dst[dst_len+i]=src[i];
	dst[dst_len+i]='\0';
	return dst;
}

int strcmp(const char *s1, const char *s2)
{
	if(s1==NULL&&s2==NULL)
		return 0;
	if(s1!=NULL&&s2==NULL)
		return 1;
	if(s1==NULL&&s2!=NULL)
		return -1;
	size_t i;
	for(i=0;s1[i]!='\0'&&s2[i]!='\0';i++){
		if(s1[i]==s2[i])
			continue;
		else if(s1[i]>s2[i])
			return 1;
		else
			return -1;
	}
	if(s1[i]=='\0'&&s2[i]=='\0')
		return 0;
	else if(s1[i]!='\0'&&s2[i]=='\0')
		return 1;
	else 
		return -1;
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	if(s1==NULL&&s2==NULL)
		return 0;
	if(s1!=NULL&&s2==NULL)
		return 1;
	if(s1==NULL&&s2!=NULL)
		return -1;
	size_t i;
	for(i=0;s1[i]!='\0'&&s2[i]!='\0'&&i<n;i++){
		if(s1[i]==s2[i])
			continue;
		else if(s1[i]>s2[i])
			return 1;
		else
			return -1;
	}
	if(i==n&&s1[i]==s2[i])
		return 0;
	if(s1[i]=='\0'&&s2[i]=='\0')
		return 0;
	else if(s1[i]!='\0'&&s2[i]=='\0')
		return 1;
	else 
		return -1;
}

void *memset(void *s, int c, size_t n)
{
	if(s==NULL||n<=0)
		return 0;
	const unsigned char u=c;
	unsigned char *us;
	for(us=s;n>0;us++,n--)
		*us=u;
	return s;
}

void *memmove(void *dst, const void *src, size_t n)
{
	if(dst==NULL||src==NULL||n<0)
		return NULL;
	unsigned char *udst=(unsigned char*)dst;
	unsigned char *usrc=(unsigned char*)src;
	size_t i;
	if(dst<src)
		for(i=0;i<n;i++)
			*(udst+i)=*(usrc+i);
	else
		for(i=n-1;i>=0;i--)
			*(udst+i)=*(usrc+i);
	return dst;
}

void *memcpy(void *out, const void *in, size_t n)
{
	if(out==NULL||in==NULL||n<0)
		return NULL;
	unsigned char *uout=(unsigned char*)out;
	unsigned char *uin=(unsigned char*)in;
	size_t i;
	for(i=0;i<n;i++)
		*(uout+i)=*(uin+i);
	return out;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
	if(s1==NULL&&s2==NULL)
		return 0;
	if(s1!=NULL&&s2==NULL)
		return 1;
	if(s1==NULL&&s2!=NULL)
		return -1;
	size_t i;
	unsigned char *us1=(unsigned char*)s1;
	unsigned char *us2=(unsigned char*)s2;
	for(i=0;i<n;i++){
		if(*(us1+i)==*(us2+i))
			continue;
		else if(*(us1+i)>*(us2+i))
			return 1;
		else
			return -1;
	}
	return 0;
}

#endif
