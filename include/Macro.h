/*
 * @Author: feiqi3
 * @Date: 2022-03-28 16:37:42
 * @LastEditTime: 2022-05-17 11:41:06
 * @LastEditors: feiqi3
 * @Description: |Some macros here|
 * @FilePath: \rayTracer\include\Macro.h
 * ->blog: feiqi3.cn <-
 */
#ifndef MACRO_H_
#define MACRO_H_
#include <string>
#include<regex>
//#define DEBUG
#define BIT(x) 1 << (x)

#define GET_CLASS_NAME(cls)                                                    \
virtual const std::string clsname() const{ return std::string(#cls); };

#define CLASS_INFO                                                             \
  virtual const std::string __className() const{                                          \
    std::string nameBuffer(__FUNCTION__);                                      \
    std::smatch result;                                                        \
    std::regex pattern("(\\w+)::");                                            \
    std::regex_search(nameBuffer, result, pattern);                            \
    return result[1];                                                          \
  }

#endif