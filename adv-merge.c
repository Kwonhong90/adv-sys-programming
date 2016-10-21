//
//  prac.c
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
    size_t size = strlen(s);
    char temp;
    
    for (size_t i = 0; i < size / 2; i++) {
        temp = s[i];
        s[i] = s[(size - 1) - i];
        s[(size - 1) - i] = temp;
    }
}

int main(int argc, char *argv[]){
    char f1_buf[BUFSIZ + 1], f2_buf[BUFSIZ + 1];
    int f1_size, f2_size, f1_size2, f2_size2, f1_sumSize = 0, f2_sumSize = 0, f1_cnt = 0, f2_cnt = 0, i = 0, j = 0;
    char *f1_buf2, *f2_buf2, *f1_stok, *f2_stok;
    char f1_buf3[256], f2_buf3[256];
    FILE *file1, *file2, *fout;
    long line1 = 0, line2 = 0, lineout = 0;
    struct timeval before, after;
    int duration;
    int ret = 1;
    
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
    f1_size = fread(f1_buf, 1, BUFSIZ, file1);
    f2_size = fread(f2_buf, 1, BUFSIZ, file2);
    f1_buf[BUFSIZ] = '\0';
    f2_buf[BUFSIZ] = '\0';
    f1_buf2 = strtok_r(f1_buf, "\n",&f1_stok);
    f2_buf2 = strtok_r(f2_buf, "\n", &f2_stok);
    while(f1_size != 0 || f2_size != 0){
        if(f1_size != 0){
            if(f1_sumSize == 0){
                f1_size2 = strlen(f1_buf2);
                f1_sumSize += (f1_size2 + 1);
            }else{
                f1_buf2 = strtok_r(NULL, "\n", &f1_stok);
                f1_size2 = strlen(f1_buf2);
                f1_sumSize += (f1_size2 + 1);
            }
            
            if(f1_sumSize == BUFSIZ + 1 || f1_sumSize == BUFSIZ){
                strcpy(f1_buf3, f1_buf2);
                f1_size = fread(f1_buf, 1, BUFSIZ, file1);
                if(f1_size != 0){
                    f1_buf[BUFSIZ] = '\0';
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
                memset(f1_buf3, 0, 256);
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
                f2_cnt++;
            }
            
            if(f2_sumSize == BUFSIZ + 1 || f2_sumSize == BUFSIZ){
                strcpy(f2_buf3, f2_buf2);
                f2_size = fread(f2_buf, 1, BUFSIZ, file2);
                if(f2_size != 0){
                    f2_buf[BUFSIZ] = '\0';
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
                memset(f2_buf3, 0, 256);
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