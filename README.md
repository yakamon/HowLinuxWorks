# 「Linux のしくみ」を読んで Linux を学ぶ。

## 感想

- Linux がプロセス、メモリ、ファイル、ストレージをどのように扱い、管理しているのかを感覚的に理解きた。
- 親切すぎる程に図と実験プログラムが豊富に用意されていて、理論というよりも直感的に Linux カーネルのやっていることを教えてくれる一冊だった。
- プロセススケジューラの節で、タイムスライシングしながら実行していく様子、また、そこに優先度を加えて実行していく様子を、実験プログラムとそのグラフ化を通して目で確認できたのが楽しかった。
- Docker コンテナだと動かせないコードがいくつかあるところが少しだけ厄介だったので、Compute Engine のインスタンスを立ててみて実験してみたりもした。
- 第 7 章のファイルシステムでは、前半は分かりやすいけど、後半は急に RAID に言及されたり紙面の都合か説明が短いなと感じる部分があって、情報技術者系の勉強したことない人だと頭に入ってきにくいだろうなと思った。