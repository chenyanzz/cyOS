#ifndef MAP_H
#define MAP_H

/*处理 字符串-任意类型 映射的库*/

typedef char *KEY;
typedef char VALUE;
/**
 * 通过keys数组的对比，返回相对应的value值
 * 每一个映射表示为 key数组与value数组下标对应
 * @param key 要检索的key
 * @param keys key数组
 * @param values value数组
 * @param isCaseSensitive 是否区分大小写,默认不区分
 * @param size 映射个数,-1的话以keys[n]=0位结尾，默认-1
 * @return 返回索引到的value,没有返回-1
 */
VALUE getValue(KEY key, KEY keys[], VALUE values[], int size, bool isCaseSensitive);

#endif //MAP_H
