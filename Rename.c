#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <direct.h>

// 定义最大路径长度常量，避免路径字符串溢出
#define MAX_PATH 512

int main(void) {
 
    //定义常量字符串指针，存储要处理的目录路径
    const char* dirpath = "C:/project/debug_test_folder";
    //定义搜索路径字符串，用于匹配目录下所有文件
    char search_path[MAX_PATH];
    // 格式化拼接搜索路径：目录路径 + /*.*
    snprintf(search_path, MAX_PATH, "%s/*.*", dirpath);

    struct _finddata_t file_info;
    // 定义文件查找句柄，用于遍历文件
    long hFind = _findfirst(search_path, &file_info);

    // 判断是否成功打开目录（句柄为 -1 表示打开失败）
    if (hFind == -1) {
        printf("打不开目录：%s\n", dirpath);// 打印错误信息，提示目录无法打开
        return 1;//返回 1 表示执行失败
    }

    // 定义文件计数器，从 1 开始，用于生成新文件名
    int idx = 1;
    // 循环遍历目录下所有文件/目录
    do {
        // 跳过系统自带的 "."目录和 ".."上级目录
        if (strcmp(file_info.name, ".") == 0 || strcmp(file_info.name, "..") == 0)
            continue;

        // 判断当前项是否为文件 _A_SUBDIR 排除目录
        if (!(file_info.attrib & _A_SUBDIR))
        {
            char old_full[MAX_PATH] = { 0 };// 定义旧文件完整路径字符串
            char new_name[MAX_PATH] = { 0 };// 定义新文件名字符串
            char new_full[MAX_PATH] = { 0 };// 定义新文件完整路径字符串
            snprintf(old_full, MAX_PATH - 1, "%s/%s", dirpath, file_info.name);// 拼接旧文件完整路径：目录路径 + 原文件名
            snprintf(new_name, MAX_PATH - 1, "dev_%03d.txt", idx);// 格式化生成新文件名：dev_ + 3 位补零数字 + .txt
            snprintf(new_full, MAX_PATH - 1 , "%s/%s", dirpath, new_name);// 拼接新文件完整路径：目录路径 + 新文件名

            if (rename(old_full, new_full) == 0)// 执行重命名操作，返回 0 表示成功，非 0 表示失败
                printf("OK: %s -> %s\n", file_info.name, new_name);// 打印成功信息：原文件名 -> 新文件名
            else
                printf("FAIL: %s\n", file_info.name); // 打印失败信息：原文件名
            idx++;
        }
    } while (_findnext(hFind, &file_info) == 0);// 计数器自增，准备处理下一个文件

    _findclose(hFind); // 关闭文件查找句柄，释放系统资源
    printf("调试版完成，共处理 %d 个文件\n", idx - 1);// 打印执行结果：总处理文件数 计数器 -1 初始值为1
    return 0;
}
