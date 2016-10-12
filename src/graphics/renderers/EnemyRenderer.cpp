#include "EnemyRenderer.h"
#include "graphics/ShaderProgram.h"
#include "graphics/SharedAssetRegistry.h"
#include "game_logic/Enemy.h"
#include "graphics/Theme.h"
#include "graphics/BitmapRenderer.h"

#define RECTANGLE3(x,y,w,h) {x,y,0,x+(w),y,0,x,y+(h),0,x+(w),y+(h),0}
#define RGBA4(r,g,b,a) {r,g,b,a,r,g,b,a,r,g,b,a,r,g,b,a}
#define SETFIELDIN4x4(a,i,v) {a[(i)] = a[(i)+4] = a[(i)+8] = a[(i)+12] = v;}
#define RECTANGLE_TEXCOORD(w,h) {0.0f,0.0f,w,0.0f,0.0f,h,w,h}

EnemyRenderer::EnemyRenderer(const SharedAssetRegistry &assetRegistry, const BitmapRenderer &bitmapRenderer)
    : mAssetRegistry(assetRegistry), mBitmapRenderer(bitmapRenderer)
{

}

void EnemyRenderer::render(const Enemy *enemy, const Theme *theme) {
    float rot = enemy->body->GetAngle();
    float sinrot = sin(rot);
    float cosrot = cos(rot);
    GLfloat pts[] = RECTANGLE3(enemy->pos.x() - Enemy::halfWidth, enemy->pos.y() + Enemy::halfHeight, Enemy::halfWidth * 2.0f, -(Enemy::halfHeight * 2 - 0.05f));
    for (int i = 0; i < 4; i++) {
        float xx = pts[i * 3] - enemy->pos.x(), yy = pts[i * 3 + 1] - enemy->pos.y();
        float xt = xx * cosrot - yy * sinrot;
        float yt = xx * sinrot + yy * cosrot;
        pts[i * 3] = xt + enemy->pos.x();
        pts[i * 3 + 1] = yt + enemy->pos.y();
    }
    GLfloat color[] = RGBA4(1, 1, 1, 1);
    GLfloat texcoord[] = RECTANGLE_TEXCOORD(1.0f, 1.0f);
    auto &defaultProgram = mAssetRegistry.defaultProgram();
    defaultProgram.bind();
    defaultProgram.setPosition(pts);
    defaultProgram.setTextureCoordinates(texcoord);
    defaultProgram.setColor(color);
    if (enemy->fast) {
        glBindTexture(GL_TEXTURE_2D, theme->getTexture("enemy-fast").id);
    }else{
        glBindTexture(GL_TEXTURE_2D, theme->getTexture("enemy").id);
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void EnemyRenderer::renderBullet(const Bullet *bullet, const Theme *theme) {
    mBitmapRenderer.renderTexture(
        theme->getTexture("bullet"),
        bullet->pos.x() - Enemy::bulletSize / 2,
        bullet->pos.y() + Enemy::bulletSize / 2,
        Enemy::bulletSize,
        Enemy::bulletSize
    );
}
