// ConsoleApplication5.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

const float DUCK_SPEED = 0.1;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load " << filename << endl;
    }
}


int main()
{
    RenderWindow window(VideoMode(800, 600), "Duck Buster");
    World world(Vector2f(0, 0));
    int score(0);
    int arrows(5);

    PhysicsSprite& crossBow = *new PhysicsSprite();
    Texture cbowTex;
    LoadTex(cbowTex, "images/crossbow.png");
    crossBow.setTexture(cbowTex);
    Vector2f sz = crossBow.getSize();
    crossBow.setCenter(Vector2f(400,
        600 - (sz.y / 2)));

    PhysicsSprite arrow;
    Texture arrowTex;
    LoadTex(arrowTex, "images/arrow.png");
    arrow.setTexture(arrowTex);
    bool drawingArrow(false);

    PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);

    PhysicsRectangle right;
    right.setSize(Vector2f(10, 600));
    right.setCenter(Vector2f(795, 300));
    right.setStatic(true);
    world.AddPhysicsBody(right);

    Texture duckTexure;
    LoadTex(duckTexure, "images/DuckFlying.png");
    PhysicsShapeList <PhysicsSprite> ducks;


    top.onCollision = [&drawingArrow, &world, &arrow]
    (PhysicsBodyCollisionResult result) {
        drawingArrow = false;
        world.RemovePhysicsBody(arrow);
        };

    Text scoreText;
    Font font;
    if (!font.loadFromFile("c:/windows/fonts/Arial.ttf")) {
        cout << "Couldn't load font arial.ttf" << endl;
        exit(1);
    }
    scoreText.setFont(font);
    Text arrowCountText;
    arrowCountText.setFont(font);

    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);
    long interval = 0;

    while((arrows > 0) || drawingArrow) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        int deltaMS = deltaTime.asMilliseconds();
        interval += deltaMS;

        if(deltaMS > 9) {
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
           
            if(Keyboard::isKeyPressed(Keyboard::Enter) &&
                !drawingArrow) {
                drawingArrow = true;
                arrow.setCenter(crossBow.getCenter());
                arrow.setVelocity(Vector2f(0, -1));
                world.AddPhysicsBody(arrow);
                arrows -= 1;
            }

            window.clear( );
            if(drawingArrow) {
                window.draw(arrow);
            }
            for(PhysicsShape& duck : ducks) {
                window.draw((PhysicsSprite&)duck);
            }

            if(interval >= 42000000) {
                cout << "count" << endl;
                
                PhysicsSprite& duck = ducks.Create();
                duck.setTexture(duckTexure);
                Vector2f sz = duck.getSize();
                duck.setCenter(Vector2f(0, 10 + (sz.y / 2)));
                duck.setVelocity(Vector2f(0.10, 0));
                world.AddPhysicsBody(duck);
                
                duck.onCollision =
                    [&drawingArrow, &world, &arrow, &duck, &ducks, &score, &right]
                    (PhysicsBodyCollisionResult result) {
                    if (result.object2 == arrow) {
                        world.RemovePhysicsBody(arrow);
                        world.RemovePhysicsBody(duck);
                        ducks.QueueRemove(duck);
                        score += 10;
                        drawingArrow = false;
                    }
                    if (result.object2 == right) {
                        world.RemovePhysicsBody(duck);
                        ducks.QueueRemove(duck);
                    }

                    };
                interval = 0;
            }

            window.draw(crossBow);
            scoreText.setString(to_string(score));
            FloatRect textBounds = scoreText.getGlobalBounds();
            scoreText.setPosition(
                Vector2f(790 - textBounds.width, 590 - textBounds.height));
            window.draw(scoreText);
            arrowCountText.setString(to_string(arrows));
            textBounds = arrowCountText.getGlobalBounds();
            arrowCountText.setPosition(
                Vector2f(10, 590 - textBounds.height));
            window.draw(arrowCountText);
           

            window.display();
            ducks.DoRemovals();
        }
    }
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    FloatRect textBounds = gameOverText.getGlobalBounds();
    gameOverText.setPosition(Vector2f(
        400 - (textBounds.width / 2),
        300 - (textBounds.height / 2)
    ));
    window.draw(gameOverText);
    window.display();
    while (true);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
