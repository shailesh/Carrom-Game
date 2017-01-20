#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<sys/time.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#define PI 3.14159265
#define eps 0.00001
using namespace std;
const int totalCoins = 9;

int equate(float a, float b) {
    if(fabs(a-b) < eps)
        return 1;
    return 0;
}

class Board {

    private:
        float board_size;
        float inner_board_size;
        int no_of_coins;
        int whiteCoins;
        int blackCoins;
        int power;
        float deceleration;
        float speed_factor;

    public:
        float theta, degTheta, pointerX, pointerY;
        double start_time;
        Board(int, float);
        void drawCircle(float, float, float, int);
        void drawLine(float, float, float, float);
        void drawTerritories();
        void showPower();
        void drawBox(float);
        void changePointerDirection(int, float, float);
        void setDegTheta(float, float, float);

        float get_inner_board_size() {
            return inner_board_size;
        }

        int get_no_of_coins() {
            return no_of_coins;
        }

        int get_power() {
            return power;
        }

        void incrementPower() {
            if(power < 9)
                power++;
        }

        void decrementPower() {
            if(power > 1)
                power--;
        }

        void resetPower() {
            power = 1;
        }

        float getDeceleration() {
            return deceleration;
        }

        float getSpeedFactor() {
            return speed_factor;
        }

};

float r = 4;
int start_timer = -1;
int finaltime = 0;

Board::Board(int coins, float boardSize) {

    no_of_coins = coins;
    board_size = boardSize + 0.4;
    inner_board_size = boardSize;
    whiteCoins = blackCoins = no_of_coins/2;
    power = 1;
    theta = 1.57;
    setDegTheta(theta, pointerX, pointerY);
    pointerX = r * cos(theta);
    pointerY = r * sin(theta);
    deceleration = -0.00010;
    speed_factor = 0.012;
    start_time = -1;

}

void Board::drawCircle(float cx, float cy, float r, int num_segments) {

    int ii;
    glPushMatrix();
    glBegin(GL_POLYGON);
    for(ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * PI * float(ii) / float(num_segments);  //get the current angle
        float x = r * cosf(theta);                                  //calculate the x component
        float y = r * sinf(theta);                                  //calculate the y component
        glVertex2f(x + cx, y + cy);                                 //output vertex
    }
    glEnd();
    glPopMatrix();

}

void Board::drawTerritories() {

    glPushMatrix();

    // Top Territory
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    drawLine(-1.0, 1.01, 1.0, 1.01);
    drawLine(-1.0, 1.09, 1.0, 1.09);
    glColor3f(1.0, 0.0, 0.0);
    drawCircle(-1.0, 1.05, 0.045, 2000);
    drawCircle(1.0, 1.05, 0.045, 2000);
    glPopMatrix();

    // Bottom Territory
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    drawLine(-1.0, -1.01, 1.0, -1.01);
    drawLine(-1.0, -1.09, 1.0, -1.09);
    glColor3f(1.0, 0.0, 0.0);
    drawCircle(-1.0, -1.05, 0.045, 2000);
    drawCircle(1.0, -1.05, 0.045, 2000);
    glPopMatrix();

    // Left Territory
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    drawLine(-1.2, 0.9, -1.2, -0.9);
    drawLine(-1.28, 0.9, -1.28, -0.9);
    glColor3f(1.0, 0.0, 0.0);
    drawCircle(-1.24, 0.9, 0.041, 2000);
    drawCircle(-1.24, -0.9, 0.041, 2000);
    glPopMatrix();

    // Right Territory
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    drawLine(1.2, 0.9, 1.2, -0.9);
    drawLine(1.28, 0.9, 1.28, -0.9);
    glColor3f(1.0, 0.0, 0.0);
    drawCircle(1.24, 0.9, 0.041, 2000);
    drawCircle(1.24, -0.9, 0.041, 2000);
    glPopMatrix();
    glPopMatrix();

}

void Board::drawLine(float ax, float ay, float bx, float by) {

    glLineWidth(2.6);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(ax, ay, 0.0);
    glVertex3f(bx, by, 0.0);
    glEnd();

}

void Board::showPower() {

    float tmp = -0.8;
    int i;
    for(i = 0 ; i < 9 ; i++) {

        if(i/3 == 0)
            glColor3f(0.4, 0.8039, 0.0);
        else if(i/3 == 1)
            glColor3f(0.80392, 0.80392, 0.0);
        else
            glColor3f(0.545, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(2.5f, tmp, 0.0);
        drawBox(0.18);
        glPopMatrix();
        tmp += 0.2;

    }

    tmp = -0.8;
    for(i = 0 ; i < power ; i++) {

        if(power <= 3)
            glColor3f(0.0, 1.0, 0.0);
        else if(power <= 6)
            glColor3f(1.0, 1.0, 0.0);
        else
            glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(2.5f, tmp, 0.0);
        drawBox(0.18);
        glPopMatrix();
        tmp += 0.2;

    }

}

void Board::drawBox(float len) {

    glBegin(GL_POLYGON);
    glVertex2f(-len / 2, -len / 2);
    glVertex2f(len / 2, -len / 2);
    glVertex2f(len / 2, len / 2);
    glVertex2f(-len / 2, len / 2);
    glEnd();

}

void Board::changePointerDirection(int tmp, float sx, float sy) {

    if(tmp == 1) {
        theta += 0.01;
    }
    else {
        theta -= 0.01;
    }
    pointerX = r * cos(theta);
    pointerY = r * sin(theta);
   
}

void Board::setDegTheta(float angle, float x, float y) { 

    float tmp = angle * 180 / PI;
    if (tmp < 0 && y > 0)
        tmp += 180;
    else if(tmp < 0)
        tmp += 360;
    else if (tmp > 0 && y < 0)
        tmp += 180;
    degTheta = tmp;

}

Board B(totalCoins, 3.2);

class Coin {

    private:
        int color;                  // Red=1 White=2 Black=3
        float x, y;                 // Coin coordinates
        float rad;

    public:
        int pocketed;               // 0 or 1
        float velx, vely;
        Coin(int, float, float);
        void createCoin();

        // Get angle of velocity with x axis between 0 to 360
        float getCoinDegTheta() {

            float tmptheta = atan2(vely, velx) * 180 / PI;
            if(tmptheta < 0)
                tmptheta += 360;
            return tmptheta;

        }

        float getCoinX() {
            return x;
        }

        float getCoinY() {
            return y;
        }

        float getCoinRad() {
            return rad;
        }

        void setCoinCoordinates(float tmpx, float tmpy) {
            x = tmpx;
            y = tmpy;
        }

        int getCoinColor() {
            return color;
        }

};

Coin::Coin(int c, float initx, float inity) {

    color = c;
    rad = 0.08;
    x = initx;
    y = inity;
    pocketed = 0;
    velx = 0.0;
    vely = 0.0;

}

void Coin::createCoin() {

    glPushMatrix();
    if(color == 1)
        glColor3f(0.8, 0.0f, 0.4);
    else if(color == 2)
        glColor3f(1.0f, 1.0f, 1.0f);
    else
        glColor3f(0.094f, 0.094f, 0.094f);
    glTranslatef(x, y, 0.0f);
    B.drawCircle(0.0, 0.0, rad, 4000);
    glPopMatrix();

}

Coin coins[] = {Coin(1, 0.0, 0.0),
    Coin(2, 0.0, 0.22),
    Coin(2, 0.0, -0.22),
    Coin(2, 0.22, 0.0),
    Coin(2, -0.22, 0.0),
    Coin(3, 0.156, 0.156),
    Coin(3, -0.156, 0.156),
    Coin(3, 0.156, -0.156),
    Coin(3, -0.156, -0.156),
};

class Pocket {

    private:
        int pno;        // ID UL=1, UR=2, LR=3, LL=4
        float x, y;
        int count;      // No of coins pocketed in this pocket
        float rad;

    public:
        Pocket(float, float, int);
        void createPocket();
        int checkObjectInPocket(float, float);

        float getPocketX() { 
            return x;
        }

        float getPocketY() {
            return y;
        }

};

Pocket::Pocket(float tmpx, float tmpy, int no) {

    x = tmpx;
    y = tmpy;
    pno = no;
    rad = 0.15;

}

void Pocket::createPocket() {

    glPushMatrix();
    glTranslatef(x, y, 0.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    B.drawCircle(0.0, 0.0, rad, 4000);
    glPopMatrix();

}

int Pocket::checkObjectInPocket(float tmpx, float tmpy) {
    float tmp = (tmpx - x) * (tmpx - x) + (tmpy - y) * (tmpy - y);
    if(tmp < rad * rad)
        return 1;
    return 0;
}

Pocket pockets[]={Pocket(-1.45, 1.45, 1),
    Pocket(1.45, 1.45, 2),
    Pocket(1.45, -1.45, 3),
    Pocket(-1.45, -1.45, 4),
};

class Striker {

    private:
        float x, y;
        int pocketed;
        float rad;

    public:    
        float velx, vely;
        int strikerHit, hideStriker;
        Striker(float, float);
        void createStriker();
        void resetStriker();

        float getStrikerX() {
            return x;
        }

        float getStrikerY() {
            return y;
        }

        float getStrikerRad() {
            return rad;
        }

        // Get angle of velocity - 0 to 360
        float getStrikerDegTheta() {

            float tmptheta = atan2(vely, velx) * 180 / PI;
            if(tmptheta < 0)
                tmptheta += 360;
            return tmptheta;

        }

        void setStrikerPosition(float tmpx, float tmpy) {
            x = tmpx;
            y = tmpy;
        }

};

timeval t1;

Striker::Striker(float tmpx, float tmpy) {

    x = tmpx;
    y = tmpy;
    pocketed = 0;
    velx = 0.0;
    vely = 0.0;
    rad = 0.05;
    strikerHit = 0;
    hideStriker = 0;

}

void Striker::createStriker() {

    if(!hideStriker) {
        glPushMatrix();
        glColor3f(0.2157f, 0.9921f, 0.8627f);
        glTranslatef(x, y, 0.0);
        B.drawCircle(0.0, 0.0, 0.10, 2000);
        glPopMatrix();
    }

}

void Striker::resetStriker() { 
    
    x = 0.0;
    y = -1.05;
    velx = 0.0;
    vely = 0.0;
    strikerHit = 0;
    B.resetPower();
    B.theta = 1.57;
    B.setDegTheta(B.theta, B.pointerX, B.pointerY);
    B.pointerX = r * cos(B.theta);
    B.pointerY = r * sin(B.theta);
    B.start_time = -1;
    hideStriker = 0;

}

Striker S(0.0, -1.05);

class Player { 

    private:
        int score;
        int color;  // Which colored coins to be pocketed

    public:
        Player(int);           
        void showScoreBoard();
        void drawDock(int, int, int);
        void incrementPlayerScore(int val) {
            score += val;
        }

        void decrementPlayerScore(int val) {
            score -= val;
        }

        void adjustScore(int col) {
            if(col == 1)
                incrementPlayerScore(50);
            else if(col == color)
                incrementPlayerScore(10);
            else
                decrementPlayerScore(5);
        }

        int getScore() {
            return score;
        }

        int getColor() {
            return color;
        }
};

Player::Player(int cl) {
    color = cl;
    score = 30;
}

void Player::showScoreBoard() {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0 );
    glRasterPos2f(-3.2, 1.7);
    char s[] = "Scoreboard";
    int i;
    for(i = 0 ; s[i] ; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0 );
    glRasterPos2f(-3.0, 1.5);
    char str[10];
    sprintf(str, "%d", score);
    for(i = 0 ; str[i] ; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0 );
    glRasterPos2f(2.35, 1.25);
    char st[] = "Power"; 
    for(i = 0 ; st[i] ; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
    glPopMatrix();

}

void Player::drawDock(int r, int w, int b) {
    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0 );
    glRasterPos2f(-2.95, 0.45);
    char str[] = "Collection";
    for(int i = 0 ; str[i] ; i++)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    glPopMatrix();

    if(r) {
        glPushMatrix();
        glColor3f(0.8f, 0.0f, 0.4f);
        glTranslatef(-2.3, -0.4, 0.0f);
        B.drawCircle(0.0, 0.0, coins[0].getCoinRad(), 4000);
        glPopMatrix();
    }
    int i;
    float tmp = -0.4;
    for(i = 0 ; i < w ; i++) {

        glPushMatrix();
        glColor3f(1.0f, 1.0f, 1.0f);
        glTranslatef(-2.7, tmp + i * 0.2, 0.0f);
        B.drawCircle(0.0, 0.0, coins[0].getCoinRad(), 4000);
        glPopMatrix();

    }

    for(i = 0 ; i < b ; i++) {

        glPushMatrix();
        glColor3f(0.094f, 0.094f, 0.094f);
        glTranslatef(-3.1, tmp + i * 0.2, 0.0f);
        B.drawCircle(0.0, 0.0, coins[0].getCoinRad(), 4000);
        glPopMatrix();

    }

}

Player P(2);

float degToRad(float tmp) {
    return tmp * PI / 180;
}

// Handle Striker vs wall collision
void handleStrikerWallCollision(float x, float y, float r) {

    if(S.hideStriker)
        return;
    float ibsize = B.get_inner_board_size() / 2 - 0.042;
    if(equate(S.velx, 0) && equate(S.vely, 0))
        return;
    float vx, vy;
    vx = S.velx;
    vy = S.vely;

    if (x + r > ibsize || x - r < -ibsize) 
        vx *= -1;
    if (y + r > ibsize || y - r < -ibsize)
        vy *= -1;

    S.velx = vx;
    S.vely = vy;
    B.theta = atan(S.vely / S.velx);
    B.setDegTheta(B.theta, S.velx, S.vely);

}

void handleStrikerInPocket(float x, float y) {

    int flag = 0;
    for(int i = 0 ; i < 4 ; i++) {
        if(pockets[i].checkObjectInPocket(x, y)) {
            flag = 1;
            break;
        }
    }
    if(flag) {
        if(!S.hideStriker)
            P.decrementPlayerScore(5);
        S.hideStriker = 1;
    }

}

void handleStrikerCoinCollision(float sx, float sy, float srad) {

    int i;
    float ux_s, ux_c, vx_s, vx_c, uy_s, uy_c, vy_s, vy_c;
    float coin_x, coin_y, tmp, newtmp;
    int decide;
    float theta;
    float cr = coins[0].getCoinRad();
    int mass_S = 2, mass_C = 1;
    for(i = 0 ; i < B.get_no_of_coins() ; i++) {
        if(coins[i].pocketed)
            continue;
        if(S.hideStriker)
            return;
        coin_x = coins[i].getCoinX();
        coin_y = coins[i].getCoinY();

        tmp = sqrt((sx - coin_x) * (sx - coin_x) + (sy - coin_y) * (sy - coin_y)) - 0.04;
        decide = 0;
        if(tmp < srad + cr) {
            newtmp = sqrt((sx + S.velx - coin_x - coins[i].velx) * (sx + S.velx - coin_x - coins[i].velx) + (sy + S.vely - coin_y - coins[i].vely) * (sy + S.vely - coin_y - coins[i].vely)) - 0.04;
            if(newtmp < srad + cr)
                decide = 1;
        }

        if(decide) {
            theta = atan((sy - coin_y) / (sx - coin_x)) * 180 / PI;
            if(theta < 0)
                theta += 180;
            theta = degToRad(theta);
            // Initial Striker velocities
            ux_s = S.velx * cosf(theta) + S.vely * sinf(theta);
            uy_s = S.vely * cosf(theta) - S.velx * sinf(theta);

            // Initial coin velocities
            ux_c = coins[i].velx * cosf(theta) + coins[i].vely * sinf(theta);
            uy_c = coins[i].vely * cosf(theta) - coins[i].velx * sinf(theta);

            vy_s = uy_s;
            vy_c = uy_c;

            vx_s = ((mass_S - mass_C) * ux_s +  2 * mass_C * ux_c) / (mass_S + mass_C);  
            vx_c = ((mass_C - mass_S) * ux_c +  2 * mass_S * ux_s) / (mass_S + mass_C);  

            S.velx = vx_s * cosf(theta) - vy_s * sinf(theta);
            S.vely = vy_s * cosf(theta) + vx_s * sinf(theta);

            coins[i].velx = vx_c * cosf(theta) - vy_c * sinf(theta);
            coins[i].vely = vy_c * cosf(theta) + vx_c * sinf(theta);

        }
    }
}

void handleCoinWallCollision() {
    float theta, ibsize = B.get_inner_board_size() / 2 - 0.01;
    float x, y, r;
    int i, prevx, prevy;

    for(i = 0; i < B.get_no_of_coins() ; i++) {

        if(coins[i].pocketed)
            continue;
        if(fabs(coins[i].velx) < 0.000001 && fabs(coins[i].vely) < 0.000001)
            continue;
        else {
            x = coins[i].getCoinX();
            y = coins[i].getCoinY();
            r = coins[i].getCoinRad();

            if (x + r > ibsize || x - r < -ibsize)
                coins[i].velx *= -1;
            if (y + r > ibsize || y - r < -ibsize)
                coins[i].vely *= -1;
        }

    }

}

void handleCoinCoinCollision() {
    int i, j;
    float crad, coin_ix, coin_iy, coin_jx, coin_jy;
    crad = coins[0].getCoinRad();
    int decide, prevx, prevy;
    float theta, tmp, newtmp;
    float acceleration = B.getDeceleration();
    float ux_i, ux_j, uy_i, uy_j, vx_i, vx_j, vy_i, vy_j;
    for(i = 0 ; i < B.get_no_of_coins() ; i++) {
        if(coins[i].pocketed)
            continue;
        for(j = i+1 ; j < B.get_no_of_coins() ; j++) {
            if(coins[j].pocketed)
                continue;
            coin_ix = coins[i].getCoinX();
            coin_iy = coins[i].getCoinY();
            coin_jx = coins[j].getCoinX();
            coin_jy = coins[j].getCoinY();
            tmp = sqrt((coin_ix - coin_jx) * (coin_ix - coin_jx) + (coin_iy - coin_jy) * (coin_iy - coin_jy));
            decide = 0;
            if(tmp < crad + crad) {
                newtmp = sqrt((coin_ix + coins[i].velx - coin_jx - coins[j].velx) * (coin_ix + coins[i].velx - coin_jx - coins[j].velx) + (coin_iy + coins[i].vely - coin_jy - coins[j].vely) * (coin_iy + coins[i].vely - coin_jy - coins[j].vely));
                if(newtmp < crad + crad)
                    decide = 1;
            }

            if(decide) {
                theta = atan((coin_iy - coin_jy) / (coin_ix - coin_jx)) * 180 / PI;
                if(theta < 0)
                    theta += 180;
                theta = degToRad(theta);

                // Initial Striker velocities
                ux_i = coins[i].velx * cosf(theta) + coins[i].vely * sinf(theta);
                uy_i = coins[i].vely * cosf(theta) - coins[i].velx * sinf(theta);

                // Initial coin velocities
                ux_j = coins[j].velx * cosf(theta) + coins[j].vely * sinf(theta);
                uy_j = coins[j].vely * cosf(theta) - coins[j].velx * sinf(theta);

                vy_i = uy_i;
                vy_j = uy_j;

                // Swap the velocities since mass of two coins is same
                vx_i = ux_j;
                vx_j = ux_i;

                coins[i].velx = (vx_i * cosf(theta) - vy_i * sinf(theta));
                coins[i].vely = (vy_i * cosf(theta) + vx_i * sinf(theta));

                coins[j].velx = (vx_j * cosf(theta) - vy_j * sinf(theta));
                coins[j].vely = (vy_j * cosf(theta) + vx_j * sinf(theta));

            }

        } 
    }
}

void handleCoinInPocket() {
    int i, j, flag;

    for(i = 0 ; i < B.get_no_of_coins() ; i++) { 
        flag = 0;
        for(j = 0 ; j < 4 ; j++) {
            if(pockets[j].checkObjectInPocket(coins[i].getCoinX(), coins[i].getCoinY())) {
                if(!coins[i].pocketed)
                    P.adjustScore(coins[i].getCoinColor());
                flag = 1;
                break;
            }
        }
        if(flag) {
            coins[i].pocketed = 1; 
        }
    }
}
// The update function to handle the multiple time frames
void update(int value) {

    float sx = S.getStrikerX();
    float sy = S.getStrikerY();
    float srad = S.getStrikerRad();
    float theta;
    float acceleration = B.getDeceleration();
    int prevx, prevy;

    // Time for v = u + a * t;
    struct timeval t;
    gettimeofday(&t, NULL);
    double time_spent = 0;
    double thistime = t.tv_sec * 1000 + t.tv_usec / 1000;

    handleStrikerInPocket(sx, sy);
    handleStrikerWallCollision(sx, sy, srad);
    handleStrikerCoinCollision(sx, sy, srad);

    handleCoinCoinCollision();
    handleCoinWallCollision();
    handleCoinInPocket();

    if(B.start_time == -1) 
        B.start_time = thistime;
    else {
        time_spent = thistime - B.start_time;
        B.start_time = thistime;
    }
    prevx = 1; prevy = 1;
    // Check previous direction of the striker prevx(x direction) +/- 1 and same for prevy
    if(S.velx < 0)
        prevx = -1;
    if(S.vely < 0)
        prevy = -1;

    // Apply friction only when the striker is moving
    if(S.velx && S.vely) {
        B.theta = atan(S.vely / S.velx);
        B.setDegTheta(B.theta, S.velx, S.vely);
        S.velx = S.velx + acceleration * cos(degToRad(B.degTheta)) * time_spent;
        S.vely = S.vely + acceleration * sin(degToRad(B.degTheta)) * time_spent;
    }

    // Make velocity 0 if the friction has increased than the objects velocity
    if((S.velx < 0 && prevx == 1) || (S.vely < 0 && prevy == 1) || (S.velx > 0 && prevx == -1) || (S.vely > 0 && prevy == -1)) {
        S.velx = 0;
        S.vely = 0;
    }

    S.setStrikerPosition(sx + S.velx, sy + S.vely);
    int i; 
    float cx, cy;
    float r = coins[0].getCoinRad();
    float ibsize = B.get_inner_board_size()/2;
    for(i = 0 ; i < B.get_no_of_coins() ; i++) {
        prevx = 1, prevy = 1;

        // Check previous direction of the striker prevx(x direction) +/- 1 and same for prevy
        if(coins[i].velx < 0)
            prevx = -1;
        if(coins[i].vely < 0)
            prevy = -1;
        acceleration = B.getDeceleration();

        // Apply friction when the coin is moving
        if(coins[i].velx && coins[i].vely) {
            theta = atan(coins[i].vely / coins[i].velx) * 180 / PI;
            if (theta < 0 && coins[i].vely > 0)
                theta += 180;
            else if(theta < 0)
                theta += 360;
            else if (theta > 0 && coins[i].vely < 0)
                theta += 180;
            theta = degToRad(theta);
            if(theta) {
                coins[i].velx = coins[i].velx + acceleration * cos(theta) * time_spent;
                coins[i].vely = coins[i].vely + acceleration * sin(theta) * time_spent;
            }
        }

        // Make velocity 0 if the friction has increased than the objects velocity
        if((coins[i].velx < 0 && prevx == 1) || (coins[i].vely < 0 && prevy == 1) || (coins[i].velx > 0 && prevx == -1) || (coins[i].vely > 0 && prevy == -1)) {
            coins[i].velx = 0;
            coins[i].vely = 0;
        }
        cx = coins[i].getCoinX() + coins[i].velx;
        cy = coins[i].getCoinY() + coins[i].vely;
        if(cx + r > ibsize) {
            coins[i].velx *= -1;
        }
        if(cy + r > ibsize) {
            coins[i].vely *= -1;
        }
        if(cx - r < -ibsize) {
            coins[i].velx *= -1;
        }
        if(cy - r < -ibsize) {
            coins[i].vely *= -1;
        }
        cx = coins[i].getCoinX() + coins[i].velx;
        cy = coins[i].getCoinY() + coins[i].vely;
        coins[i].setCoinCoordinates(cx, cy);
    }

    // Reset the striker if velocity of all the objects is 0
    int flag = 0; 
    int count = 0;
    int red = 0;
    for(i = 0 ; i < B.get_no_of_coins() ; i++) {
        if(coins[i].velx && coins[i].vely)
            flag = 1;
        if(coins[i].pocketed && P.getColor() == coins[i].getCoinColor())
            count++;
        if(coins[i].getCoinColor() == 1)
            red = 1;
    }
    if(!flag && !S.velx && !S.vely && S.strikerHit) {
        S.resetStriker();
    }
    if(count == B.get_no_of_coins() / 2 && red) {
        cout << "*******************************************************************************\n";
        cout << "                               Your Score: " << P.getScore() << endl;
        cout << "*******************************************************************************\n";
        exit(0);
    }

    glutTimerFunc(10, update, 10);

}

//Called when a key is pressed
void handleKeyPress(unsigned char key, int x, int y) {   

    float sx = S.getStrikerX();
    float sy = S.getStrikerY();

    switch(key)
    {
        case 27:
            exit(0);
        case 32:
            {
                if(S.strikerHit)
                    return;
                float pwr = B.get_power();
                float m = (B.pointerY - sy) / (B.pointerX - sx);
                float tmptheta = atan(m) * 180 / PI;
                float speed = B.getSpeedFactor();
                S.velx = pwr * speed / sqrt(1 + m * m);
                S.vely = pwr * speed * m / sqrt(1 + m * m);

                // To avoid the problem: range of atan = (-PI/2, PI/2)
                if ((tmptheta < 0 && B.pointerY > -1.05) || (tmptheta > 0 && B.pointerY < -1.05)) {
                    S.velx = -S.velx;
                    S.vely = -S.vely;
                }
                S.strikerHit = 1;
                glutTimerFunc(10, update, 1);
                break;
            }
        case 97:
            {
                B.changePointerDirection(1, sx, sy);
                break;
            }
        case 99:
            {
                B.changePointerDirection(2, sx, sy);
                break;
            }
    }

}

void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,
            double(w) / double(h),
            1.0,
            200.0);

}

void drawBoard() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             //Switch to the drawing perspective
    glLoadIdentity();                       //Reset the drawing perspective

    float ibsize = B.get_inner_board_size();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glPushMatrix();

    // Outer Square
    glPushMatrix();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glColor3f(0.0f, 0.0f, 0.0f);
    B.drawBox(ibsize + 0.4);

    // Inner Square
    glColor3f(1.0f, 0.894f, 0.7686f);
    B.drawBox(ibsize);

    // Outer Circle
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(4.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    B.drawCircle(0, 0, 0.40, 2000);

    // Inner Circle
    glLineWidth(2.2f);
    glColor3f(0.0f, 0.0f, 0.0f);
    B.drawCircle(0, 0, 0.35, 4000);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glPopMatrix();

    // Spokes
    ibsize /= 1.5;
    glPushMatrix();
    int angle = 0, tmp = 0;
    for(angle = 0 ; angle <= 360 ; angle += 45) {
        tmp++;

        glPushMatrix();
        glLineWidth(1.1f);
        glRotatef((float)angle, 0.0, 0.0, 1.0);
        glTranslatef(0.0f, 0.0f, -1.088f);
        glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_LOOP);
        glVertex3f(0, 0, 0);
        glVertex3f(0, ibsize * 0.18, 0);
        glVertex3f(ibsize * 0.02, ibsize * 0.05, 0);
        glEnd();
        if(tmp%2 == 1)
            glColor3f(1.0f, 0.0f, 0.0f);
        else
            glColor3f(0.0f, 0.0f, 0.0f);
        glBegin(GL_POLYGON);
        glVertex3f(0, 0, 0);
        glVertex3f(0, ibsize * 0.18, 0);
        glVertex3f(-ibsize * 0.02, ibsize * 0.05, 0);
        glEnd();
        glPopMatrix();
    }
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 0.6470, 0.39);
    glTranslatef(-2.7, 0.0, 0.0);
    B.drawBox(1.2);
    glPopMatrix();
    int i, red = 0, black = 0, white = 0;
    for(i = 0 ; i < B.get_no_of_coins() ; i++) {
        if(coins[i].pocketed) {
            if(coins[i].getCoinColor() == 1)
                red++;
            else if(coins[i].getCoinColor() == 2)
                white++;
            else
                black++;
        }
    }
    P.drawDock(red, white, black);

    // Create territories
    glPushMatrix();
    B.drawTerritories();
    glPopMatrix();

    // Create Pockets
    glPushMatrix();
    for(int i = 0 ; i < 4 ; i++)
        pockets[i].createPocket();
    glPopMatrix();

    // Create coins
    glPushMatrix();
    for(i=0; i<totalCoins ; i++) {
        if(!coins[i].pocketed)
            coins[i].createCoin();
    }

    glPopMatrix();

    // Create Striker
    glPushMatrix();
    S.createStriker();
    glPopMatrix();

    // Show Power
    glPushMatrix();
    B.showPower();
    glPopMatrix();

    // Pointer
    float sx = S.getStrikerX();
    float sy = S.getStrikerY();

    if(!S.strikerHit) {                                     // If strikerHit draw the line else not
        glPushMatrix();
        glColor3f(0.5, 0.6, 0.1);
        B.drawLine(sx, sy, B.pointerX , B.pointerY); 
        glPopMatrix();
    }

    P.showScoreBoard();
    glPopMatrix();

    // Decrement score according to start time of the game
    if(!S.strikerHit) {
        timeval t2;
        gettimeofday(&t2, NULL);
        P.decrementPlayerScore(t2.tv_sec - t1.tv_sec);
        t1 = t2;
    }
    glutSwapBuffers(); //Send the 3D scene to the screen

}

// Set if mouse is down and moving
int dragflag = 0;
void onKeyPress(int key, int x, int y) {

    float sx = S.getStrikerX();
    float sy = S.getStrikerY();
    switch(key) {

        case GLUT_KEY_LEFT:
            {
                // Decrement sx only if striker on the territory  
                if(sx >= -1.0 && fabs(sy + 1.05) < eps)
                    sx -= 0.05;
                break;
            }
        case GLUT_KEY_RIGHT:
            {
                // Increment sx only if striker on the territory  
                if(sx <= 1.0 && fabs(sy + 1.05) < eps)
                    sx += 0.05;
                break;
            }
        case GLUT_KEY_UP:
            {
                // Increment power only if striker is not hit
                if(!S.strikerHit)
                    B.incrementPower();
                break;
            }
        case GLUT_KEY_DOWN:
            {
                // Decrement power only if striker is not hit
                if(!S.strikerHit)
                    B.decrementPower();
                break;
            }
    }
    S.setStrikerPosition(sx, sy);
    glutPostRedisplay();

}

// Called when click is pressed and mouse is moving
void mouseButton(int tmpx, int tmpy) {

    float x = tmpx - 1365 / 2.0;
    float y = 766 / 2.0 - tmpy;
    if(dragflag && fabs(x / 299.5 * 1.6) <= 1)
        S.setStrikerPosition(x / 299.5 * 1.6 , -1.05);

}

// Called when click is pressed
void setDragFlag(int button, int state, int x, int y) {

    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        dragflag = 1;
    else
        dragflag = 0;

    if(button == GLUT_LEFT_BUTTON) {
        if(!S.strikerHit)
        {
            float sx = S.getStrikerX(), sy = S.getStrikerY();
            float pwr = B.get_power();
            float m = (B.pointerY - sy) / (B.pointerX - sx);
            float tmptheta = atan(m) * 180 / PI;
            float speed = B.getSpeedFactor();
            S.velx = pwr * speed / sqrt(1 + m * m);
            S.vely = pwr * speed * m / sqrt(1 + m * m);

            // To avoid the problem: range of atan = (-PI/2, PI/2)
            if ((tmptheta < 0 && B.pointerY > -1.05) || (tmptheta > 0 && B.pointerY < -1.05)) {
                S.velx = -S.velx;
                S.vely = -S.vely;
            }
            S.strikerHit = 1;
            glutTimerFunc(10, update, 1);
        }
    }
}

// Called when hovering the mouse over the board
void mouseHover(int tmpx, int tmpy) {

    float x = tmpx - 1365 / 2.0;
    float y = 767 / 2.0 - tmpy;
    if(fabs(x / 299.5 * 1.6) <= 1.7 && fabs(y / 295.5 * 1.6) <= 1.7)
    {
        B.pointerX = x / 299.5 * 1.6;
        B.pointerY = y / 295.5 * 1.6;
        B.theta = atan(B.pointerY / B.pointerX);
        B.setDegTheta(B.theta, B.pointerX, B.pointerY);
    }
    glutPostRedisplay();

}

int main(int argc, char** argv) {

    // Global timer for decreasing score
    if(!S.strikerHit)
        gettimeofday(&t1,NULL);

    // Init Window 
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);

    glutCreateWindow("Carrom Game");
    // Make Wnidowsize full screen
    glutFullScreen();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   // Setting a background color

    glutDisplayFunc(drawBoard);
    glutIdleFunc(drawBoard);

    glutKeyboardFunc(handleKeyPress);
    glutSpecialFunc(onKeyPress);
    glutReshapeFunc(handleResize);

    // Mouse handlers
    glutPassiveMotionFunc(mouseHover);
    glutMouseFunc(setDragFlag);
    glutMotionFunc(mouseButton);
    // Main loop
    glutMainLoop();
    return 0;

}
