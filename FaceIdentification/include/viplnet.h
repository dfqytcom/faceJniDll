#ifndef VIPLNET_H_
#define VIPLNET_H_

#if defined(__unix__) || defined(__APPLE__)

#ifndef fopen_s

#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL

#endif //fopen_s

#endif //__unix__, __APPLE__

#endif //VIPLNET_H_
