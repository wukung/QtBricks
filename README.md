# QtBricks

QtBricks 是一款使用 **C++17** 與 **Qt 6 Widgets** 打造的經典打磚塊 (Breakout) 遊戲。專案採用物件導向架構與 CMake 建置，並內建準確的物理碰撞引擎與單元測試系統，適合作為學習 Qt 開發與遊戲物理設計的參考專案。

## 🌟 遊戲特色 (Features)

- **現代化遊戲物理架構**：
  - **物理子步進 (Sub-stepping)**：將每一幀的運算切分為不大於 5ms 的微小步長，徹底解決球體在高速移動下的「穿透現象 (Tunneling)」。
  - **最小重疊量演算法 (Penetration Depth)**：摒棄單純的座標邊界判斷，改由重疊深度判定碰撞接觸面，確保反彈方向絕對精確。
  - **變動時間步長 (Variable Time Step)**：利用 `QElapsedTimer` 取得精準的 `dt`，使得遊戲速度不受畫面更新率 (FPS) 波動的影響。
- **動態計分與排行榜 (Leaderboard)**：
  - 根據磚塊所在的層數給予不同的動態分數（頂層 50 分至底層 10 分）。
  - 利用 `QSettings` 實作本地成績持久化，每次破關或死亡後自動依據「分數最高、時間最短」進行排序，並於結算畫面展示 **Top 5** 玩家排行榜。
- **測試驅動保護 (TDD / Unit Tests)**：
  - 專案導入 **Qt Test** 單元測試框架。
  - 將遊戲核心抽象為 `QtBricksCore` 靜態函式庫，獨立測試 `Ball`、`Paddle`、`Brick` 與 `ScoreManager` 等邏輯，保護邊界防禦與排序機制免受回歸 Bug 影響。

## 🛠️ 開發與建置環境 (Requirements)

- **C++ Standard**: C++17 或以上
- **Framework**: Qt 6 (包含 Widgets 與 Test 模組)
- **Build System**: CMake (>= 3.16)
- **OS**: 支援跨平台 (macOS, Windows, Linux)

## 🚀 如何編譯與執行 (Build & Run)

1. **取得程式碼**
   ```bash
   git clone https://github.com/wukung/QtBricks.git
   cd QtBricks
   ```

2. **使用 CMake 進行編譯**
   ```bash
   cmake -B build
   cmake --build build
   ```

3. **執行遊戲**
   - **macOS**: `open build/QtBricks.app`
   - **Windows / Linux**: `./build/QtBricks`

4. **執行單元測試 (Unit Tests)**
   ```bash
   cd build
   ctest --output-on-failure
   ```

## 🎮 操作說明 (Controls)

- **開始遊戲 / 重新開始**：`Space` (空白鍵)
- **移動擋板**：`←` (左方向鍵) / `→` (右方向鍵)
- **離開遊戲**：`Esc`

## 📂 專案架構與開發文件

- 核心邏輯存放於 `src/` 與 `include/`。
- 單元測試存放於 `UT/`。
- 詳細的技術演進與架構設計規劃，請參閱 [`doc/design.md`](doc/design.md)。
