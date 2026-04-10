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

    /** \brief Destrutkor - dealokacja zasobów + wyzerowanie właściwości
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

    void render( float x, float y );

    int width_get();
    int height_get();
    bool load_check();

protected:

private:
    SDL_Texture* texture;
    int texture_width;
    int texture_height;
};

#endif // OBJECT_H
