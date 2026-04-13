/** \author Igor Solny
 *
 * \file Object.h
 *
 * \brief Wszystko co związane z obiektami w grze
 *
 */


#ifndef OBJECT_H
#define OBJECT_H

#include "../include/SDL3/SDL.h"

/** \class Object
 *
 *  \brief Klasa obiektu w grze
 */
class Object
{
public:

    /** \brief Konstruktor - inicjacja właściwości klasy
     *
     *
     */
    Object();

    /** \brief Destruktor - dealokacja zasobów + wyzerowanie właściwości
     *
     *
     */
    ~Object();

    /** \brief Ładuje teksturę z pilku
     *
     * \param path const char* - ścieżka do pliku (podajemy linuxowo: "folder/plik")
     * \return bool - prawda jeśli udało się załadować plik, fałsz jeśli się nie udało
     *
     */
    bool texture_load( const char* path );

    /** \brief dealokacja zasobów
     *
     * \return void
     *
     */
    void destroy();

    /** \brief render obiektu na ekran (kordynaty liczone od lewego gornego rogu zawsze, oś y, rośnie w dół okna!)
     *
     * \param x float - współrzędna x okna
     * \param y float - współrzędna y okna
     * \param 0.0 double angle - kąt obrotu
     * \param nullptr SDL_FPoint* center - środek obrotu
     * \return void
     *
     */
    void render( float x, float y, double angle = 0.0, SDL_FPoint* center = nullptr);

    /** \brief getter
     *
     * \return int - szerokość obiektu
     *
     */
    int width_get();

    /** \brief getter
     *
     * \return int - wysokość obiektu
     *
     */
    int height_get();
   // bool load_check();

protected:

private:
    SDL_Texture* texture;/**< wskaźnik do tekstury */
    int texture_width;/**< szerokość tekstury */
    int texture_height;/**< wysokość tekstury */
};



#endif // OBJECT_H
