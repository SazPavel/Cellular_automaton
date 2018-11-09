#include <SFML/Window.hpp> 
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "windows.h"

#define TILESIZE 1
int n = 300;
int N = n*n;
using namespace std;

class TileMap : public sf::Drawable, public sf::Transformable{
public:

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height)
    {
        // загрузка текстуры
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // изменить размер массива вершин, чтобы он соответствовал размеру уровня
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // заполнить массив вершин, с одним квадратом на плиту
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // получить текущий номер плитки
                int tileNumber = tiles[i + j * width];

                // найти свое положение в текстуре плитки
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // получить указатель на квадрант текущей плитки
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // определить его 4 угла
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // определить его 4 текстурные координаты
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // применить преобразование
        states.transform *= getTransform();

        // применить текстуру плитки
        states.texture = &m_tileset;

        // нарисуйте массив вершин
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};



int main(){
	
    // создать окно
    sf::RenderWindow window(sf::VideoMode(n * TILESIZE, n * TILESIZE), "Automat");
    // определить уровень с массивом индексов плитки

  
	//скрыть консоль
	HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    
    // run the main loop
	window.setKeyRepeatEnabled(false);
	
	
    TileMap map;
    int level[N];
    int level1[N];
		
    srand(time(NULL));
	for(int i = 0; i < N; i++){		 
		level1[i] = 0;
		level[i] = rand()%8; 
	}
    if (!map.load("resources/tileset3.png", sf::Vector2u(TILESIZE, TILESIZE), level, n, n))
        return -1;
        
	window.clear();  
	window.draw(map); 
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
        	if(event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
    			if (event.mouseButton.button == sf::Mouse::Left)
    				for(int i = 0; i < N; i++){
						level[i] = rand()%8; 
						level1[i] = 0;
					}
		}    
		
        for(int i = 0; i < N; i++){
			int next = (level[i] + 1)%7;
			level1[i] = level[i];
			
        	if(i%n != n-1 && level[i+1] == next)
        		level1[i] = next;
        	if(i%n != 0 && level[i-1] == next)
        		level1[i] = next;
        	if(i >= n && level[i-n] == next)
        		level1[i] = next;
        	if(i <= N - n - 1 && level[i+n] == next)
        		level1[i] = next;

        	
		 /*	if(i >= n && i%n != n-1 && level[i+1-n] == next)
        		level1[i] = next;
        	if(i >= n && i%n != 0 && level[i-1-n] == next)
        		level1[i] = next;
       		if(i <= N - n - 1&& i%n != n-1 && level[i+1+n] == next)
        		level1[i] = next;
        	if(i <= N - n - 1&& i%n != 0 && level[i-1+n] == next)
        		level1[i] = next;*/
        					
		}  
		for(int i = 0; i < N; i++) {
			level[i] = level1[i];
			level1[i] = 0;
		}
    	if (!map.load("resources/tileset3.png", sf::Vector2u(TILESIZE, TILESIZE), level, n, n))
        	return -1;
        Sleep(100);	
		window.clear();  
		window.draw(map); 			
       	window.display();
       	
    } 
    	
    return 0;
}
