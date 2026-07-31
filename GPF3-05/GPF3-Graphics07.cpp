#include "FrameBufferEmulator.h"

// buff  フレームバッファの先頭アドレス
// width, height  フレームバッファの横幅と高さ
// radius  円の半径
// centerX, centerY  中心座標(X, Y)
// color  描画色（B,G,Rの配列）
void drawFilledCircle(unsigned char* buff, int width, int height, int radius, int centerX, int centerY, unsigned char color[3])
{
    const int squaredRadius = radius * radius; // 半径の二乗
    for (int y = 0; y < height; y++) {         // 縦方向のループ
        int squaredY = y - centerY;            // Y軸の中心からの距離
        squaredY *= squaredY;                  // 二乗しておく
        for (int x = 0; x < width; x++) {      // 横方向のループ
            int squaredX = x - centerX;        // X軸の中心からの距離
            squaredX *= squaredX;              // 二乗しておく
            if (squaredY + squaredX <= squaredRadius) { // 距離が半径以下なら（二乗どうしで比較）
                // 円の内側として色を置く
                *buff++ = color[0]; // B
                *buff++ = color[1]; // G
                *buff++ = color[2]; // R
            }
            else {
                buff += 3; // 円の外側（色は置かず次のピクセルへ）
            }
        }
    }
}

int centerX;
int centerY;
int radius;
int green;

void FrameBufferEmulator::initUser()
{
    centerX = width / 2;  // Xの中心座標
    centerY = height / 2; // Yの中心座標
    radius = 100;         // 半径
    green = 200;          // 緑色の値
}

void FrameBufferEmulator::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
    unsigned char color[3] = { 10, green, 0 }; // B, G, R
    switch (keyLevel)   // ← 第三段階までは keyTrigger。第四段階で keyLevel に切替（押しっぱなしで連続変化）
    {
        case SDLK_UP:    // 上矢印キー
            radius++;    // 半径を大きくする
            break;
        case SDLK_DOWN:  // 下矢印キー
            radius--;    // 半径を小さくする
            break;
        case SDLK_RIGHT: // 右矢印キー
            green++;     // 緑色の値を増やす
            break;
        case SDLK_LEFT:  // 左矢印キー
            green--;     // 緑色の値を減らす
            break;
        default:
            break;
    }
    drawFilledCircle(buff, width, height, radius, centerX, centerY, color);
}
