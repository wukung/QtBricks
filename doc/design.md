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

所有的遊戲邏輯與物件定義已重構並獨立為 `Ball`, `Paddle`, `Brick` 三個 C++ 類別，而 `GameWidget` 負責實例化並管理它們，降低了整體的耦合度。

### 3.1. 擋板 (Paddle)
- **類別**：`Paddle` (`Paddle.h`, `Paddle.cpp`)
- **資料結構**：使用 `QRectF` 定義擋板的位置與長寬，並內部維護狀態 (`moveLeft`, `moveRight`, `moveSpeed`)。
- **控制與更新**：提供 `setMovingLeft()` / `setMovingRight()` 供外部呼叫。呼叫 `update(dt, windowWidth)` 時，類別會根據內部狀態平滑移動，並確保不超出邊界。提供 `draw(QPainter*)` 方法自行渲染。

### 3.2. 球 (Ball)
- **類別**：`Ball` (`Ball.h`, `Ball.cpp`)
- **資料結構**：使用 `QRectF` 定義邊界框，輔以 `speedX` 與 `speedY` 記錄每秒移動速度。
- **運動更新**：
  - `move(float step)`：負責以指定的子步長時間平移球體。
  - `GameWidget` 負責呼叫此方法並於外部偵測視窗邊界碰撞，藉由 `setSpeedX`、`setSpeedY` 等 setter 改變其運動方向。提供 `draw(QPainter*)` 方法自行渲染。

### 3.3. 磚塊 (Brick)
- **類別**：`Brick` (`Brick.h`, `Brick.cpp`)
- **資料結構**：包含 `QRectF rect` (磚塊區域)、`QColor brickColor` (顏色) 與 `bool destroyed` (擊破狀態)。
- **操作**：提供 `setDestroyed(bool)` 等狀態修改介面，並由 `draw(QPainter*)` 負責根據其狀態與顏色進行繪製。

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

## 5. 單元測試 (Unit Testing)

為了確保遊戲物理與核心邏輯的穩定性，本專案已導入 **Qt Test** 單元測試框架：
- **架構設計**：將 `Ball`, `Paddle`, `Brick` 等核心物件編譯成靜態函式庫 `QtBricksCore`，使得主程式與測試程式能共用核心邏輯。
- **測試涵蓋範圍**：
  - `tst_ball.cpp`：測試球體的初始化、依據時間步長 (`dt`) 的平移邏輯與強制位移方法。
  - `tst_paddle.cpp`：測試擋板的狀態機（左右移動）、速度更新以及預期位置。
  - `tst_brick.cpp`：測試磚塊的擊破狀態修改與初始配置。
- **執行方式**：編譯完成後，可於 `build` 目錄下執行 `ctest` 自動跑完所有測試案例。

## 6. 未來擴充方向 (Future Work)

為了讓這款打磚塊遊戲更加完整且具備豐富的遊玩體驗 (Game Feel)，未來可考慮擴充以下功能：

1. **進度與計分系統**：
   - 實作分數統計 (Score System) 與分數浮動動畫。
   - 連擊與分數加成系統 (Combo System)：球未落回擋板前連續擊破磚塊給予倍率加成。
   - 生命值系統 (Lives System)：給予玩家多次重試機會。
   - 排行榜 (Leaderboard)：紀錄並展示最高分。
2. **場景與關卡機制**：
   - 關卡編輯器 (Level Editor)：讓玩家能自訂磚塊配置。
   - 多元關卡設計：動態載入不同地圖，甚至加入 Boss 戰或動態移動的干擾障礙物。
   - 不同材質的磚塊：需多次撞擊才能擊破的硬磚塊、或是擊破後會爆炸影響周圍的炸彈磚塊。
3. **掉落道具系統 (Power-ups / Debuffs)**：
   - 增益：擋板加長、多顆球 (Multi-ball)、火力球 (貫穿)、磁鐵擋板、雷射槍。
   - 減益：擋板變短、球速加快、操作方向反轉。
4. **多媒體與視覺回饋 (Game Feel / Juice)**：
   - 音效與背景音樂 (BGM / SFX)：碰撞、過關、吃到道具時的對應音效。
   - 背景圖 (Background Image)：豐富單調的純色背景。
   - 粒子特效 (Particle Effects)：磚塊破裂時噴發碎片、球體高速移動時的殘影。
   - 畫面震動 (Screen Shake) 與 碰撞形變 (Squash and Stretch)：增強物理碰撞的打擊感。
