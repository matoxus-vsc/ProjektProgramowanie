/** \author Igor Solny
 *
 * \file structs.h
 *
 * \brief Globalne struktury, enumy, itd. używane w innych plikach
 *
 */


#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

/** \enum Game_state
 *
 *  \brief Stany gry
 */
enum Game_state
{
    PLAYING
};

/** \struct Vec2i
 *
 *  \brief Wektor 2 wymiary inty
 */
struct Vec2i
{
    int x;
    int y;

    /** \brief ustawienie structa
     *
     * \param xx int - x
     * \param yy int - y
     * \return void
     *
     */
    void arg_set(int xx, int yy)
    {
        x = xx;
        y = yy;
    }

    /** \brief Konstruktor bazowy - tylko by móc wstawić wektor w klasie
     *
     *
     */
    Vec2i(){};

    /** \brief Konstruktor do tworzenia wektora
     *
     * \param xx int - x
     * \param yy int - y
     *
     */
    Vec2i(int xx, int yy)
    {
        arg_set(xx, yy);
    }

    /** \brief Konstruktor do kopiowania wektora
     *
     * \param p const Vec2i& - wektor
     *
     */
    Vec2i(const Vec2i& p)
    {
        arg_set(p.x, p.y);
    }
};

/** \struct Vec2f
 *
 *  \brief Wektor 2 wymiary floaty
 */
struct Vec2f
{
    float x;
    float y;

    /** \brief ustawienie wektora
     *
     * \param xx float - x
     * \param yy float - y
     * \return void
     *
     */
    void arg_set(float xx, float yy)
    {
        x = xx;
        y = yy;
    }

    /** \brief Konstruktor bazowy - tylko by móc wstawić wektor w klasie
     *
     *
     */ Vec2f(){};

    /** \brief Konstruktor do tworzenia wektora
     *
     * \param xx float - x
     * \param yy float - y
     *
     */
    Vec2f(float xx, float yy)
    {
        arg_set(xx, yy);
    }

    /** \brief Konstruktor do kopiowania wektora
     *
     * \param p const Vec2f& - wektor
     *
     */
    Vec2f(const Vec2f& p)
    {
        arg_set(p.x, p.y);
    }
};

#endif // STRUCTS_H_INCLUDED
