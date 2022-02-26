#include <MapTools.h>

int getMemPermission(unsigned long address){
    char line[PATH_MAX] = {0};
    char perms[5];
    int bol = 1;

    FILE *fp = fopen("/proc/self/maps", "r");
    if (fp == nullptr){
        return 0;
    }

    while (fgets(line, PATH_MAX, fp) != nullptr){
        unsigned long start, end;
        int eol = (strchr(line, '\n') != nullptr);
        if (bol){
            if (!eol){
                bol = 0;
            }
        } else {
            if (eol){
                bol = 1;
            }
            continue;
        }
        if (sscanf(line, "%lx-%lx %4s", &start, &end, perms) != 3){
            continue;
        }
        if (start <= address && address < end){
            int prot = 0;
            if (perms[0] == 'r'){
                prot |= PROT_READ;
            } else if (perms[0] != '-'){
                goto unknown_perms;
            }
            if (perms[1] == 'w'){
                prot |= PROT_WRITE;
            } else if (perms[1] != '-'){
                goto unknown_perms;
            }
            if (perms[2] == 'x'){
                prot |= PROT_EXEC;
            } else if (perms[2] != '-'){
                goto unknown_perms;
            }
            if (perms[3] != 'p'){
                goto unknown_perms;
            }
            if (perms[4] != '\0'){
                perms[4] = '\0';
                goto unknown_perms;
            }
            fclose(fp);
            return prot;
        }
    }
unknown_perms:
    fclose(fp);
    return 0;
}

// 修改内存段权限 : 注入后 部分目标内存段可能无权限读写 eg. xa cb
bool editMemProt(unsigned long address, int prot){
    void *page_start = (void *)(address - address % PAGE_SIZE);
    if (-1 == mprotect(page_start, PAGE_SIZE, prot)){
        return false; // 修改内存段保护属性失败
    } else {
        return true; // 修改内存段保护属性成功
    }
}