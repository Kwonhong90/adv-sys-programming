//
//  adv-merge.c
//
//
//  Created by 민권홍 on 2016. 10. 18..
//
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

void reverseString(char* s) {
    int size = strlen(s);
    int index;
    char temp;
    
    for (index = 0; index < size / 2; index++) {
        temp = s[index];
        s[index] = s[(size - 1) - index];
        s[(size - 1) - index] = temp;
    }
}

int main(int argc, char *argv[]){
    
    int f1_size, f2_size, f1_size2, f2_size2;
    int f1_sumSize = 0, f2_sumSize = 0;
    int i, j;
    
    /*
     * f1_buf, f2_buf 는 파일에서 읽는 변수를 저장하기 위한 공간
     * f1_buf2, f2_buf2 는 token 으로 나눌 때 strtok_r 에서 반환하는 문자열을 사용하기 위한 변수
     * f1_buf3, f2_buf3 는 앞에서 읽은 버퍼 마지막 부분이 1줄이 채워지지 않았을 경우 뒤에서 읽은 부분과 합치기 위한 변수
     * f1_stok, f2_stok 은 strtok_r 의 현재 위치를 저장하기 위한 변수
     */
    char f1_buf[BUFSIZ + 1], f2_buf[BUFSIZ + 1], f1_buf3[BUFSIZ], f2_buf3[BUFSIZ];
    char *f1_buf2, *f2_buf2;
    char *f1_stok, *f2_stok;
    
    // FILE 1, 2 에 대한 파일 포인터와 output 파일에 대한 FILE 포인터
    FILE *file1, *file2, *fout;
    
    // 실행 시간과 LINE 수를 출력하기 위한 변수들을 선언
    int duration;
    int ret = 1;
    long line1 = 0, line2 = 0, lineout = 0;
    struct timeval before, after;
    
    
    if (argc != 4) {
        fprintf(stderr, "usage: %s file1 file2 fout\n", argv[0]);
        goto leave0;
    }
    if ((file1 = fopen(argv[1], "rt")) == NULL) {
        perror(argv[1]);
        goto leave0;
    }
    if ((file2 = fopen(argv[2], "rt")) == NULL) {
        perror(argv[2]);
        goto leave1;
    }
    if ((fout = fopen(argv[3], "wt")) == NULL) {
        perror(argv[3]);
        goto leave2;
    }
    gettimeofday(&before, NULL);
    
    // FILE 1, 2에서 최초로 BUFSIZ 만큼의 문자를 읽는다
    f1_size = fread(f1_buf, 1, BUFSIZ, file1);
    f2_size = fread(f2_buf, 1, BUFSIZ, file2);
    
    // \0을 마지막에 안채울경우 문법오류가 발생
    f1_buf[BUFSIZ] = '\0';
    f2_buf[BUFSIZ] = '\0';
    
    f1_buf2 = strtok_r(f1_buf, "\n",&f1_stok);
    f2_buf2 = strtok_r(f2_buf, "\n", &f2_stok);
    
    while((f1_size != 0) || (f2_size != 0)){
        if(f1_size != 0){
            if(f1_sumSize == 0){
                f1_size2 = strlen(f1_buf2);
                f1_sumSize += (f1_size2 + 1);
            }else{
                f1_buf2 = strtok_r(NULL, "\n", &f1_stok);
                f1_size2 = strlen(f1_buf2);
                f1_sumSize += (f1_size2 + 1);
            }
            
            if((f1_sumSize == BUFSIZ + 1) || (f1_sumSize == BUFSIZ)){
                strcpy(f1_buf3, f1_buf2);
                f1_size = fread(f1_buf, 1, BUFSIZ, file1);
                if(f1_size != 0){
                    f1_buf2 = strtok_r(f1_buf, "\n", &f1_stok);
                    f1_size2 = strlen(f1_buf2);
                    
                    if(f1_buf2[13] != ':'){
                        
                        strcat(f1_buf3, f1_buf2);
                        f1_sumSize = (f1_size2 + 1);
                    }
                    else{
                        f1_sumSize = 0;
                    }
                    
                }
                reverseString(f1_buf3);
                fprintf(fout, "%s\n", f1_buf3);
            }
            else{
                reverseString(f1_buf2);
                fprintf(fout, "%s\n", f1_buf2);
                
            }
            line1++;
            lineout++;
        }
        if(f2_size != 0){
            if(f2_sumSize == 0){
                f2_size2 = strlen(f2_buf2);
                f2_sumSize += (f2_size2 + 1);
            }else{
                f2_buf2 = strtok_r(NULL, "\n",&f2_stok);
                f2_size2 = strlen(f2_buf2);
                f2_sumSize += (f2_size2 + 1);
            }
            
            if((f2_sumSize == BUFSIZ + 1) || (f2_sumSize == BUFSIZ)){
                strcpy(f2_buf3, f2_buf2);
                f2_size = fread(f2_buf, 1, BUFSIZ, file2);
                if(f2_size != 0){
                    f2_buf2 = strtok_r(f2_buf, "\n", &f2_stok);
                    f2_size2 = strlen(f2_buf2);
                    
                    if(f2_buf2[13] != ':'){
                        
                        strcat(f2_buf3, f2_buf2);
                        f2_sumSize = (f2_size2 + 1);
                    }
                    else{
                        f2_sumSize = 0;
                    }
                    
                }
                reverseString(f2_buf3);
                fprintf(fout, "%s\n", f2_buf3);
            }
            else{
                reverseString(f2_buf2);
                fprintf(fout, "%s\n", f2_buf2);
                
            }
            line2++;
            lineout++;
        }
    }
    gettimeofday(&after, NULL);
    
    duration = (after.tv_sec - before.tv_sec) * 1000000 + (after.tv_usec - before.tv_usec);
    printf("Processing time = %d.%06d sec\n", duration / 1000000, duration % 1000000);
    printf("File1 = %ld, File2= %ld, Total = %ld Lines\n", line1, line2, lineout);
    ret = 0;
    
leave3:
    fclose(fout);
leave2:
    fclose(file2);
leave1:
    fclose(file1);
leave0:
    return ret;
}