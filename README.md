# 房屋中介管理系统 (House Management System)

## 项目概述

这是一个基于MFC和SQL Server的房屋中介管理系统，提供房屋信息管理、客户信息管理等功能。系统采用数据库驱动架构，支持数据的持久化存储和高效查询。

## 技术架构

### 开发环境
- **IDE**: Visual Studio 2022 Community
- **框架**: MFC (Microsoft Foundation Classes)
- **数据库**: SQL Server
- **编程语言**: C++
- **平台**: Windows x64

### 架构设计
- **MVC模式**: 采用Model-View-Controller架构
- **数据库驱动**: 使用ADO.NET进行数据库操作
- **模块化设计**: 房屋管理和客户管理独立模块

## 功能模块

### 1. 房屋管理模块
- **增删改查**: 完整的房屋信息CRUD操作
- **搜索筛选**: 支持按地址、类型、状态等条件搜索
- **数据验证**: 输入数据的格式和唯一性校验
- **导入导出**: 支持CSV格式数据导入导出

### 2. 客户管理模块
- **客户信息管理**: 客户基本信息的增删改查
- **搜索功能**: 支持按姓名、电话、地址等条件搜索
- **数据校验**: 身份证号、电话号码等格式验证
- **唯一性检查**: 客户ID和身份证号的唯一性校验

### 3. 系统功能
- **用户界面**: 基于MFC的现代化界面设计
- **数据持久化**: SQL Server数据库存储
- **错误处理**: 完善的异常处理和用户提示

## 数据库设计

### 房屋表 (House)
```sql
CREATE TABLE House (
    HouseID INT PRIMARY KEY IDENTITY(1,1),
    Address NVARCHAR(200) NOT NULL,
    Type NVARCHAR(50),
    Area FLOAT,
    Price FLOAT,
    Status NVARCHAR(20),
    Description NVARCHAR(500)
);
```

### 客户表 (Customers)
```sql
CREATE TABLE Customers (
    CustomerID NVARCHAR(50) PRIMARY KEY,
    Name NVARCHAR(100) NOT NULL,
    IDCard NVARCHAR(20),
    Phone NVARCHAR(20),
    Email NVARCHAR(100),
    Address NVARCHAR(200),
    Occupation NVARCHAR(50),
    Income NVARCHAR(50),
    Preference NVARCHAR(200),
    Status NVARCHAR(20),
    Remark NVARCHAR(500)
);
```

## 核心类设计

### 数据库操作类

#### HouseDBHelper (房屋数据库操作)
- 静态方法设计，简化调用
- 支持房屋信息的增删改查
- 提供搜索和唯一性校验功能

#### CCustomerDBHelper (客户数据库操作)
- 实例方法设计，支持连接管理
- 完整的客户信息CRUD操作
- 数据库连接池管理

### 界面类

#### CHouseManagementNEWView (主视图)
- 标签页式界面设计
- 房屋和客户列表显示
- 右键菜单和双击编辑

#### CHouseEditDlg / CCustomerEditDlg (编辑对话框)
- 数据输入和验证
- 支持新增和编辑模式
- 实时数据校验

## 开发进度

### 已完成功能
1. **房屋管理模块** ✅
   - 房屋信息的增删改查
   - 房屋信息搜索和筛选
   - 数据库存储和检索
   - 界面操作和交互

2. **客户管理模块** ✅
   - 客户信息的增删改查
   - 客户信息搜索和筛选
   - 数据库存储和检索
   - 界面操作和交互
   - 客户ID和身份证号唯一性校验

### 技术特点
- 使用MFC框架构建Windows桌面应用
- 采用SQL Server数据库存储数据
- 实现数据库连接池和错误处理
- 支持数据导入导出功能
- 提供友好的用户界面

### 编译错误修复记录

#### 2025年1月修复记录
1. **HouseManagementNEWDoc.cpp错误修复**
   - 删除旧的客户管理vector相关代码
   - 移除未定义的字段和方法引用

2. **CustomerSearchDlg.cpp和CustomerEditDlg.cpp错误修复**
   - 统一字段名为数据库字段名
   - 修正数据类型不匹配问题

3. **HouseEditDlg.cpp错误修复**
   - 修正类名错误（CHouseDBHelper改为HouseDBHelper）
   - 调整为静态方法调用

4. **HouseManagementNEWView.cpp错误修复**
   - 统一方法调用为实例方法
   - 修正客户管理相关方法调用

5. **CustomerDBHelper.cpp错误修复**
   - 重写数据库操作实现
   - 修正SQL语句和CRecordset使用
   - 使用CDBVariant正确读取字段

6. **客户管理界面空白问题修复** (2025年1月最新修复)
   - 修复客户列表控件创建时缺少WS_VISIBLE标志
   - 修正OnTcnSelchangeTabCtrl方法中的OnSize调用参数
   - 添加客户表自动创建功能
   - 添加测试数据自动生成功能
   - 改进标签页切换逻辑
   - 添加调试信息和错误处理

### 待开发功能
1. **租赁管理模块** 🔄
   - 租赁合同管理
   - 租金收取记录
   - 租期管理

2. **管理员模块** 🔄
   - 用户权限管理
   - 系统设置
   - 数据备份恢复

3. **统计分析模块** 🔄
   - 房屋出租率统计
   - 收入统计报表
   - 客户分析报告

## 使用说明

### 环境要求
1. **SQL Server**: 安装并配置SQL Server数据库
2. **Visual Studio**: 安装Visual Studio 2022 Community
3. **MFC支持**: 确保项目包含MFC库

### 数据库配置
1. 创建数据库：`HouseManagement`
2. 配置连接字符串：修改`GetConnStr()`函数中的连接参数
3. 运行表创建脚本：系统会自动创建必要的表

### 编译运行
1. 打开`HouseManagementNEW.sln`解决方案
2. 选择`Debug x64`配置
3. 编译项目：`Ctrl+Shift+B`
4. 运行程序：`F5`

## 技术特点

### 设计模式
- **单例模式**: 数据库连接管理
- **工厂模式**: 对话框创建
- **观察者模式**: 数据更新通知

### 代码质量
- **SOLID原则**: 遵循面向对象设计原则
- **错误处理**: 完善的异常处理机制
- **代码注释**: 详细的中文注释
- **命名规范**: 统一的命名约定

### 性能优化
- **数据库连接池**: 高效的连接管理
- **查询优化**: 索引和SQL优化
- **内存管理**: 智能指针和RAII

## 项目结构

```
HouseManagementNEW/
├── 核心文件/
│   ├── HouseManagementNEW.cpp      # 应用程序入口
│   ├── HouseManagementNEWDoc.cpp   # 文档类
│   └── HouseManagementNEWView.cpp  # 视图类
├── 数据库操作/
│   ├── HouseDBHelper.h/cpp         # 房屋数据库操作
│   └── CustomerDBHelper.h/cpp      # 客户数据库操作
├── 界面组件/
│   ├── HouseEditDlg.h/cpp          # 房屋编辑对话框
│   ├── CustomerEditDlg.h/cpp       # 客户编辑对话框
│   ├── HouseSearchDlg.h/cpp        # 房屋搜索对话框
│   └── CustomerSearchDlg.h/cpp     # 客户搜索对话框
├── 资源文件/
│   ├── res/                        # 图标和资源
│   └── HouseManagementNEW.rc       # 资源文件
└── 配置文件/
    ├── HouseManagementNEW.sln      # 解决方案文件
    └── HouseManagementNEW.vcxproj  # 项目文件
```

## 维护说明

### 代码维护
- 定期检查编译警告和错误
- 保持代码风格一致性
- 及时更新文档和注释

### 数据库维护
- 定期备份数据库
- 监控数据库性能
- 优化查询语句

### 版本控制
- 使用Git进行版本管理
- 记录重要的代码变更
- 保持提交信息的规范性

## 联系方式

如有问题或建议，请联系开发团队。

---

**最后更新**: 2024年12月
**版本**: 2.0.0
**状态**: 开发中 

## 租赁合同编辑对话框（LeaseEditDlg）
- 用于新增和编辑租赁合同。
- 字段：
  - 房屋编号（下拉框选择，自动加载所有房屋）
  - 客户编号（下拉框选择，自动加载所有客户）
  - 起租日期、退租日期、租金、押金、状态、备注（常规输入）
- 支持数据校验，防止输入错误。
- 操作成功/失败有友好提示。
- 与主界面联动，支持合同的增删改查。 