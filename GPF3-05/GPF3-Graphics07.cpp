#include "FrameBufferEmulator.h"

// buff フレームバッファの先頭アドレス
// width, height フレームバッファの高さと横幅
// radius 円の半径
// centerX, centerY 中心座標(X, Y)
// color 描画色（B,G,Rの配列）
void drawFilledCircle(unsigned char* buff, int width, int height, int radius, int centerX, int centerY, unsigned char color[3])
{
    const int squaredRadius = radius * radius; // 半径の二乗
    for (int y = 0; y < height; y++) { // 縦方向のループ
        int squaredY = y - centerY; // Y軸の中心からの距離
        squaredY *= squaredY; // 二乗しておく
        for (int x = 0; x < width; x++) { // 横方向のループ
            int squaredX = x - centerX; // X軸の中心からの距離
            squaredX *= squaredX; // 二乗しておく
            if (squaredY + squaredX <= squaredRadius) { // 距離が半径以下ならば
                *buff++ = color[0]; // B
                *buff++ = color[1]; // G
                *buff++ = color[2]; // R
            }
            else {
                buff += 3; // 円の外側
            }
        }
    }
}

int centerX;
int centerY;
int radius;
int colorIndex; // 色のインデックス（左右キーで変える）

// 色のパレット（B, G, R）
const unsigned char palette[][3] = {
    {  0, 200,  10 }, // 緑
    {  0,   0, 255 }, // 赤
    { 255,   0,   0 }, // 青
    {  0, 255, 255 }, // 黄
    { 255, 255,   0 }, // シアン
    { 255,   0, 255 }, // マゼンタ
};
const int paletteSize = sizeof(palette) / sizeof(palette[0]);

void FrameBufferEmulator::initUser()
{
    centerX = width / 2;
    centerY = height / 2;
    radius = 100;
    colorIndex = 0;
}

void FrameBufferEmulator::drawUser(unsigned char* buff, int mode, int keyLevel, int keyTrigger)
{
    if (keyTrigger == SDLK_UP) { // 上矢印キーで半径を大きく
        radius++;
    }
    if (keyTrigger == SDLK_DOWN) { // 下矢印キーで半径を小さく
        if (radius > 1) { // 半径が0以下にならないようにする
            radius--;
        }
    }
    if (keyTrigger == SDLK_RIGHT) { // 右矢印キーで次の色へ
        colorIndex = (colorIndex + 1) % paletteSize;
    }
    if (keyTrigger == SDLK_LEFT) { // 左矢印キーで前の色へ
        colorIndex = (colorIndex - 1 + paletteSize) % paletteSize;
    }

    unsigned char color[3] = {
        palette[colorIndex][0],
        palette[colorIndex][1],
        palette[colorIndex][2]
    };
    drawFilledCircle(buff, width, height, radius, centerX, centerY, color);
}
