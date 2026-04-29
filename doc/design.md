# QtBricks 遊戲設計與實作文件

本文件記錄了 QtBricks (打磚塊遊戲) 的核心設計與實作細節。專案採用 C++ 撰寫，以 Qt 6 作為 UI 與渲染框架，並使用 CMake 進行專案建置。

## 1. 系統架構

為了保持專案的簡單與易讀性，本遊戲目前將 MVC (Model-View-Controller) 的概念整合在一個單一的視窗類別 `GameWidget` 中：
- **Model (資料模型)**：由 `GameWidget` 內部的私有成員變數維護，例如球的速度、位置，擋板位置，以及所有磚塊的狀態。
- **View (視圖)**：利用 Qt 的 `paintEvent` 進行遊戲畫面的重新渲染。
- **Controller (控制器)**：透過 Qt 的 `keyPressEvent` 和 `keyReleaseEvent` 接收使用者輸入，並透過 `QTimer` (`timerEvent`) 進行遊戲狀態的定期更新。

## 2. 核心技術選型

- **建置工具**：CMake (最低版本 3.16)，可跨平台生成專案設定。
- **圖形渲染**：使用 Qt 的 2D 繪圖 API `QPainter` 配合 `QPaintEvent` 進行畫面繪製。
- **遊戲迴圈與時間**：採用 `QTimer` 以大約 60 FPS 的頻率更新畫面，但背後引入 `QElapsedTimer` 計算真實經過時間 (`dt`)，並採用變動時間步長 (Variable Time Step) 來確保遊戲速度不受幀率波動影響。

## 3. 遊戲物件與實作細節

所有的遊戲邏輯與物件定義皆封裝於 `GameWidget` 類別中。

### 3.1. 擋板 (Paddle)
- **資料結構**：使用 `QRectF` 定義擋板的位置與長寬。
- **控制方式**：透過監聽左右方向鍵 (`Qt::Key_Left`, `Qt::Key_Right`) 更新布林值 `moveLeft` 與 `moveRight`。在物理子步進 (Sub-stepping) 迴圈中，根據這些布林值與 `dt` 平滑地移動擋板位置，並限制其不可超出視窗左右邊界。

### 3.2. 球 (Ball)
- **資料結構**：使用 `QRectF` 定義球的邊界框，輔以 `ballSpeedX` 與 `ballSpeedY` 記錄球在 x 軸與 y 軸上的每秒移動像素速度。
- **運動與邊界碰撞**：
  - 為了避免高速下發生穿透 (Tunneling) 現象，程式在 `updateGame(float dt)` 中實作了**物理子步進 (Sub-stepping)**。將每個畫面的經過時間切割成最大不超過 5ms 的小步長。
  - 每個小步長將球平移 `(ballSpeedX * step, ballSpeedY * step)`。
  - 當球碰到視窗左、右、上邊界時，會將速度向量反轉，實現反彈效果。
  - 若球碰到下邊界，則判定遊戲結束 (`gameOver = true`)。

### 3.3. 磚塊 (Bricks)
- **資料結構**：定義了一個簡單的 `Brick` 結構體，包含 `QRectF rect` (磚塊區域) 與 `bool destroyed` (是否已被擊破)。
- **初始化**：在 `initGame()` 中，程式會產生 5 列 10 行共 50 個磚塊，並根據行列加上間距排列於畫面上方。

### 3.4. 碰撞偵測 (Collision Detection)
遊戲使用 AABB 碰撞偵測機制 (`QRectF::intersects`) 結合**最小重疊量演算法 (Penetration Depth)**：
- **計算方法**：當球與其他物件（擋板或磚塊）發生重疊時，計算球的四個邊界與物件邊界的重疊深度 (Overlap)，並取重疊量最小的一面作為真實的撞擊面，以此決定反彈方向。這徹底解決了單純依靠球的座標判斷所導致的反彈方向邏輯矛盾。
- **球與擋板**：
  - 除了最小重疊量判定外，當球擊中擋板上方時，會依據擊中點距離擋板中心的相對位置，改變球的 `x` 軸速度 (`ballSpeedX`)。這提供玩家控制球反彈角度的能力。
- **球與磚塊**：
  - 偵測到球與未擊破的磚塊重疊時，將磚塊標記為已擊破 (`destroyed = true`)。
  - 透過最小重疊量得知撞擊面後，程式會自動反轉對應的 `ballSpeedY` 或 `ballSpeedX`，並將球稍微推離碰撞面以避免連鎖卡死。

## 4. 遊戲狀態管理

遊戲包含三個主要的布林狀態變數：
- `gameStarted`：判斷玩家是否已按下空白鍵開始遊戲。
- `gameOver`：判斷球是否掉落到底部，遊戲失敗。
- `gameWon`：判斷是否所有磚塊皆已被擊破。

在畫面渲染 (`paintEvent`) 中，會根據這些狀態繪製相對應的文字提示（如 "Press Space to Start", "Game Over", "You Win!"）。

## 5. 未來擴充方向 (Future Work)

此設計架構簡單明瞭，若未來需要擴充，可以考慮以下方向：
1. **物件導向重構**：將 Ball, Paddle, Brick 獨立成不同的 C++ 類別，降低 `GameWidget` 的耦合度。
2. **場景管理**：加入關卡設計 (Level Design) 與分數統計 (Score System)。
3. **特效與道具**：增加磚塊的硬度設計（需要多次撞擊）、或是掉落能改變擋板長度或球速的道具。
