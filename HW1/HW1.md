## 1-15
- (a)
    1.利用I/O來傳輸資料
    2.CPU傳遞資料到device的local buffer來讓設備動作
    3.監控特定記憶體的變化來採取動作
    4.利用polling與vectored interrupt來提高整體效率
- (b)
    裝置會發送Interrupt來通知cpu已經完成

- (c)
    會干擾
    - 情況一
        同時訪問相同的內存
    - 情況二
        同時訪問造成帶寬的競爭
    - 情況三
        因為DMA占用大量的帶寬而產生的延遲
## 2-15
1. Share Memory
    - strength
        1. 共享內存速度比較快
        2. 可以達到緊密同步
    - weakness
        1. 同步會比較複雜
        2. 難擴展
2. Message Passing
    - strength
        1. 比較簡單
        2. 沒有同步問題
        3. 可擴展
    - weakness
        1. 相比於共享記憶體需要花費比較多的資源
## 2-19
1. 可以更加容易擴充、更容易維護
2. 用message passing來進行互動
3. 比單一kernel的性能開銷高     

## 3-12
些換前保存目前的state，然後讀取之前別的process的state來進行動作

## 3-18
- ordinary pipes are more suitable than named pipes

在任務相近的情況，如讀寫文件的時候

- named pipes are more suitable than ordinary pipes

在任務不相近的時候，如讀寫文件跟上網同時進行(鬆耦合)


