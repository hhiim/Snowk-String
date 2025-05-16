# Snowk-String Library

[![License: WTFPL](https://img.shields.io/badge/License-WTFPL-brightgreen.svg)](http://www.wtfpl.net/about/)
[![C++](https://img.shields.io/badge/C++-23-blue.svg)](https://isocpp.org/)

C++ 端序无关字符串库，属于 Snowk 项目的一部分。  
目前功能还不完善，**搜索、替换、比较、截取、拼接、格式化、正则表达式** 功能还不存在，  
而且 C++ 版本很快会随着 Snowk 语言自举而被弃用。

## 特性

- 支持 UTF-8/16/32 和 ASCII 编码
- 端序无关的存储与操作（Big/Little Endian）
- 类型安全的 `static_cast` 扩展
- 小型字符串优化（SSO）支持
- 编译期类型检查与概念约束

## 快速开始

```cpp
#include "string.hpp"
#include "instances/UTF8.h"

using namespace Snowk;
using namespace Snowk::internal;

int main() {
    // 创建UTF-8字符串
    Snowk::string<UTF8> s1(u8"Hello 你好");
    
    // 转换为UTF-16 Big Endian
    Snowk::string<UTF16, endian::big> s2(s1);
    
    // 迭代访问
    for (auto ch : s2) {
        // do something
    }
    return 0;
}
```

## 项目结构

```python
include/
├── encode/        # 编码抽象层
├── endian/        # 端序处理核心
├── instances/     # 具体编码实现
├── metaprogram/   # 类型操作工具
└── string.hpp     # 主接口
```

## 构建/使用

首先，在你的项目中创建一个私有仓库。

```python
your-project
  - private-repo
    - packages
      - s/snowk-string
      - ... # 更多依赖
    # 你自己的源代码
  - xmake.lua
```
切换回 Snowk-String 源代码根目录，  
执行 `xmake package`，将 `/build/packages/...`  
的内容拷贝到私有仓库的 `.../packages/...` 目录下。

在你的 xmake.lua 文件中添加：
```lua
-- 第一个是仓库名，第二个是路径
add_repositories("private-repo private-repo")
```

## TODO 计划

### 核心功能
- [ ] 实现跨编码转换算法
- [ ] 添加运行时端序自动检测机制



### 质量保证
- [ ] 完善 Cathat2 单元测试
- [ ] 实现代码覆盖率检测
- [ ] 完善文档注释（Doxygen）

### 功能扩展
- [ ] 添加正则表达式支持
- [ ] 实现字符串格式化操作
- [ ] 支持自定义编码扩展接口

## 贡献指南

它随时可能被弃用，所以不推荐提交任何贡献。  
但你可以提出 issues 请求……
