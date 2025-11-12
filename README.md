# Qt Calculator 計算機

一個使用 Qt5 開發的計算機應用程式，**不使用 .ui 檔案**，純程式碼實現 GUI。

## 功能特點

- ✅ 支援浮點數運算
- ✅ 支援基本四則運算：加法 (+)、減法 (-)、乘法 (*)、除法 (/)
- ✅ 支援小數點輸入
- ✅ 清除功能 (C)
- ✅ 美觀的 GUI 介面
- ✅ 防止除以零的錯誤處理

## 編譯與執行

### 系統需求

- Qt5 開發工具包
- C++ 編譯器 (g++ 或類似)
- qmake

### 安裝依賴 (Ubuntu/Debian)

```bash
sudo apt-get install qt5-qmake qtbase5-dev qtbase5-dev-tools
```

### 編譯

```bash
qmake calculator.pro
make
```

### 執行

```bash
./calculator
```

## 專案結構

```
.
├── calculator.pro    # Qt 專案檔案
├── main.cpp          # 應用程式入口點
├── calculator.h      # Calculator 類別標頭檔
├── calculator.cpp    # Calculator 類別實作
└── README.md         # 本檔案
```

## 使用說明

1. 點擊數字按鈕 (0-9) 輸入數字
2. 點擊小數點按鈕 (.) 輸入小數
3. 點擊運算子按鈕 (+, -, *, /) 選擇運算
4. 點擊等號按鈕 (=) 計算結果
5. 點擊清除按鈕 (C) 重置計算機

## 技術實作

- 使用 `QWidget` 作為主視窗
- 使用 `QLineEdit` 作為顯示器
- 使用 `QPushButton` 作為按鈕
- 使用 `QGridLayout` 排列按鈕
- **完全不使用 .ui 檔案**，所有 GUI 元件都在程式碼中創建

## 特色

- 支援連續運算（例如：5 + 3 + 2 = 10）
- 錯誤處理（除以零時顯示 "Error"）
- 自適應視窗大小
- 彩色按鈕設計