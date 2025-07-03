# OcfEngine
新しくエンジンを開発中

OcfEngineは、C++17で開発されているクロスプラットフォームゲームエンジンです。2D/3Dレンダリング、オーディオ、入力処理、UI機能を提供します。

## 特徴
- **クロスプラットフォーム**: Windows、Linuxをサポート
- **モダンなC++**: C++17標準を使用
- **2D/3Dレンダリング**: OpenGL ベースのレンダリングシステム
- **オーディオエンジン**: OpenAL を使用した3Dオーディオ
- **GUI**: ボタン、チェックボックス、スライダーなどのUI要素
- **コンポーネントシステム**: 移動、衝突判定などのコンポーネント
- **テストスイート**: 包括的なテストプログラム

## システム要件

### 共通要件
- CMake 3.8 以上
- C++17対応コンパイラ (GCC 7+, Clang 5+, MSVC 2017+)

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install -y build-essential cmake pkg-config
sudo apt install -y libwayland-dev wayland-protocols libxkbcommon-dev
sudo apt install -y libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt install -y libpng-dev libbz2-dev libbrotli-dev
```

### Windows
- Visual Studio 2017以上 または MinGW-w64
- CMake 3.8以上

## ビルド方法

### Linux / macOS
```bash
# リポジトリをクローン
git clone https://github.com/arriveenet/OcfEngine.git
cd OcfEngine

# ビルドディレクトリを作成
mkdir build
cd build

# CMakeでプロジェクト設定
cmake ..

# ビルド実行 (並列ビルドで高速化)
make -j4
```

### Windows (Visual Studio)
```bash
# リポジトリをクローン
git clone https://github.com/arriveenet/OcfEngine.git
cd OcfEngine

# ビルドディレクトリを作成
mkdir build
cd build

# Visual Studioプロジェクトファイル生成
cmake ..

# Visual Studioでビルド、または
cmake --build . --config Release
```

### CMakeオプション
- `OCF_BUILD_TESTS=ON/OFF`: テストプログラムをビルドするかどうか (デフォルト: ON)

## テスト実行

ビルド成功後、テストプログラムを実行できます：

```bash
# ビルドディレクトリから
./tests/cpp-tests/cpp-tests
```

### テスト内容
- **ActionTest**: アクションシステムのテスト
- **AudioEngineTest**: オーディオエンジンのテスト
- **ComponentTest**: コンポーネントシステムのテスト
- **DrawShapeTest**: 図形描画のテスト
- **EventTest**: イベントシステムのテスト
- **FontTest**: フォントレンダリングのテスト
- **MeshRendererTest**: 3Dメッシュレンダリングのテスト
- **SpriteTest**: 2Dスプライトのテスト
- **TiledMapTest**: タイルマップのテスト

## 使用ライブラリ

- **OpenGL**: グラフィックスレンダリング
- **GLFW**: ウィンドウ管理・入力処理
- **OpenAL**: オーディオ処理
- **GLM**: 数学ライブラリ
- **FreeType**: フォントレンダリング
- **libpng**: PNG画像処理
- **TinyXML2**: XML処理
- **fmt**: フォーマット処理
- **tinyobjloader**: OBJファイル読み込み


## トラブルシューティング

### ビルドエラー

**依存関係エラー**
```
fatal error: X11/Xlib.h: No such file or directory
```
→ X11開発ヘッダーをインストール: `sudo apt install libx11-dev`

**Wayland関連エラー**
```
Failed to find wayland-scanner
```
→ Wayland開発パッケージをインストール: `sudo apt install libwayland-dev wayland-protocols`

**CMakeバージョンエラー**
```
CMake Deprecation Warning
```
→ CMake 3.10以上にアップグレードを推奨

### 実行時エラー

**アセットが見つからない**
- `assets/` フォルダがビルドディレクトリから相対パスで見つからない場合
- プロジェクトルートディレクトリから実行してください

## 開発への貢献

1. このリポジトリをフォーク
2. 機能ブランチを作成 (`git checkout -b feature/amazing-feature`)
3. 変更をコミット (`git commit -m 'Add amazing feature'`)
4. ブランチにプッシュ (`git push origin feature/amazing-feature`)
5. プルリクエストを作成

## ライセンス

このプロジェクトのライセンス情報については、リポジトリ内のライセンスファイルを参照してください。
