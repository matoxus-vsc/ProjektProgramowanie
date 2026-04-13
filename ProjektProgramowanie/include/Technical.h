/** \author Igor Solny
 *
 * \file Technical.h
 *
 * \brief Technikalia SDL
 */

#ifndef TECHNICAL_H
#define TECHNICAL_H

#include "../include/SDL3/SDL.h"

/** \enum Errors_technical
 *
 *  \brief Błędy przy inicjacji SDL
 */
enum Errors_technical
{
    NO_ERROR = 0x000,
    ERR_SDL_INIT = 0x001,
    ERR_SDL_CREATE_WINDOW = 0x002,
    ERR_SDL_GET_WINDOW_SIZE = 0x003,
    ERR_SDL_CREATE_RENDERER = 0x004
};


/** \class Technical
 *
 *  \brief Technikalia, głównie SDL
 */
class Technical
{
    public:

        /** \brief Konstruktor - inicjacja właściwości klasy
         *
         *
         */
        Technical();

        ~Technical();


        /** \brief Inicjacja SDL
         *
         * \param t Technical&
         * \return friend bool
         *
         */
        friend bool init(Technical& t);

        /** \brief Dealokacja po skończonej grze
         *
         * \param t Technical&
         * \return friend void
         *
         */
        friend void close(Technical& t);

        /** \brief getter
         *
         * \return SDL_Window* - wskaźnik do okna
         *
         */
        SDL_Window* window_get();

        /** \brief getter
         *
         * \return int - szerokość okna
         *
         */
        int window_width_get();

        /** \brief getter
         *
         * \return int - wysokość okna
         *
         */
        int window_height_get();

        /** \brief getter
         *
         * \return SDL_Renderer* - wskaźnik do renderera
         *
         */
        SDL_Renderer* renderer_get();

        /** \brief getter
         *
         * \return Errors_technical - kod błędu
         *
         */
        Errors_technical err_code_get();

    protected:

    private:
        SDL_Window* window;/**< Wskaźnik do okna gry */
        int window_width;/**< Szerokość okna gry */
        int window_height;/**< Wysokość okna gry */

        SDL_Renderer* renderer;/**< Wskaźnik do renderera okna gry */

        Errors_technical err_code;/**< Błąd przy inicjacji */

};

/** \brief Inicjacja technikalii SDL
 *
 * \param t Technical& - obiekt klasy z object_init.h
 * \return bool - fałsz, gdy wystąpi błąd na którymś etapie, w przeciwnym razie prawda
 *
 */
bool init(Technical& t);

/** \brief Załadowanie mediów - tekstur, audio itp. z plików
 *
 * \return bool - fałsz, gdy wystąpi błąd na którymś etapie, w przeciwnym razie prawda
 *
 */
bool media();


/** \brief Czyszczenie po zakończeniu gry
 *
 * \return void
 *
 */
void close(Technical& t);


#endif // TECHNICAL_H
