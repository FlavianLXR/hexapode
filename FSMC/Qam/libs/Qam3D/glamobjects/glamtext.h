/*  ---------------------------------------------------------------------------
 *  filename    :   glamtext.h
 *  description :   INTERFACE de la classe GLamText
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2020 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam3D/GLam Qt Library"
 *
 *  This program is free software ;  you can  redistribute it and/or  modify it
 *  under the terms of the  GNU General Public License as published by the Free
 *  Software Foundation ; either version 3 of the License, or  (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
 *  FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
 *  more details.
 *
 *	You should have  received  a copy of the  GNU General Public License  along
 *	with this program. If not, see <http://www.gnu.org/licenses/>.
 *  ---------------------------------------------------------------------------
 */

#ifndef GLAMTEXT_H
#define GLAMTEXT_H

// objet 'texte 3D' dérivé de GLamGroup
// chaque élément du groupement est un GLamSolid représentant
// un caractère...

#include "glamgroup.h"
#include <QFont>

class GLamText : public GLamGroup
{
  public:
	GLamText(const QString& text = "?" ) ;
	GLamText(const QString& text, const QFont& font ) ;
	GLamText(const GLamText& obj ) ;
	~GLamText() ;
	GLamText& operator =(const GLamText& obj ) ;


    /*! Propriété @ref text_text "text". */
    void setText(const QString& text ) ;
    /*! Propriété @ref text_font "font". */
    void setFont(const QFont &font ) ;
    void setFontByPath(const QString& fontPath ) ;
    /*! Propriété @ref text_size "fontSize". */
    void setFontSize(qreal size ) ;
    /*! Propriété @ref text_thickness "thickness". */
    void setThickness(qreal thickness ) ;

    /*! Propriété @ref text_text "text". */
    QString text() const { return m_text ; }
    /*! Propriété @ref text_font "font". */
    QFont font() const { return m_font ; }
    /*! Propriété @ref text_size "fontSize". */
    qreal fontSize() const { return m_font.pointSizeF() ; }
    /*! Propriété @ref text_thickness "thickness". */
    qreal thickness() const { return m_thickness ; }

    /*! Retourne la largeur totale du texte. */
    qreal width() const { return m_rect.left() + m_concatWidth ; }
    /*! Retourne la hauteur du texte. */
    qreal height() const { return ascent() + descent() ; }

    // new v0.91
    void concat(GLamText& text, QColor simpleColor = QColor() ) ;

    virtual void setMaterial(GLamMaterial material ) ;
    virtual GLamMaterial& material() ;

    virtual void draw(GLamObject* parent = 0 ) ;
    //

  private:
    qreal ascent() const { return -m_rect.top() ; }
    qreal descent() const { return m_rect.height() + m_rect.top() ; }
    void loadDefaultFont() ;
	void buildCharAsSolid() ;

  private:
	QString			m_text ;
	QFont			m_font ;
	qreal			m_thickness ;
	QRectF			m_rect ;
    // new v0.91
    QVector<int>    m_iObj ;
    GLamMaterial    m_material ;
    bool			m_materialChanged ;
    float           m_concatWidth ;
} ;

#endif	// GLAMTEXT_H

/*!
  @class GLamText
  @brief Texte 3D.

La classe GLamText permet de créer des chaines de caractères sous forme
de solides 3D ; l'épaisseur du texte est maintenue par la propriété @c thickness,
la taille des caractères par la propriété @c fontSize et le style des glyphes
par la propriété @c font.
@n Un objet GLamText peut subir des concaténations d'objets de même type ;
la largeur totale du texte est donnée par &c width(), sa hauteur par @c height().

<hr><h2>Propriétés</h2>

<p>@anchor text_text @c text : chaine de caractères.
- QString @b text () const ;
- void @b setText (const QString &a text ) ;

<p>@anchor text_font @c font : police de caractères (Arial par défaut).
- QFont @b font () const ;
- void @b setFont (const QFont &a font ) ;
- void @b setFontByPath (const QString& &a fontPath ) ;

<p>@anchor text_size @c fontSze : taille des caractères (propriété @c pointSizeF
de la police de caractères) égale à 3 par défaut.
- qreal @b fontSize () const ;
- void @b setFontSize (qreal &a fontSize ) ;

<p>@anchor text_thickness @c thickness : épaisseur de matière (0 par défaut).
- qreal @b thickness () const ;
- void @b setThickness (qreal &a thickness ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamtext_example.png" width=400px </td>
<td>
@code
// attributs
    GLamText*   m_text[2] ;

// construction
    m_text[0] = new GLamText( "C++/Qt" ) ;
    m_text[0]->setThickness(0.5) ;
    m_text[0]->defaultMatrix().scale(0.1) ;

    m_text[1] = new GLamText( *m_text[0] ) ;
    m_text[1]->setText("Qam") ;
    m_text[1]->setFontSize( 2 ) ;
    m_text[1]->material().setColor(0, 0.5, 0.7 ) ;

    GLamText t("3D") ;
    m_text[1]->concat( t, QColor(0, 180, 50 ) ) ;

    t.setText(" Library") ;
    t.material().set( m_text[1]->material() ) ;
    m_text[1]->concat( t ) ;

    m_text[0]->setFontByPath("<pathtofont>/Papyrus.ttc") ;

    float decalx = -m_text[0]->width() / 2.0 ;
    float decaly = -m_text[0]->height() / 2.0 ;
    m_text[0]->defaultMatrix().translate( decalx, decaly + 2, -0.2 ) ;

    decalx = -m_text[1]->width() / 2.0 ;
    decaly = -m_text[1]->height() / 2.0 ;
    m_text[1]->defaultMatrix().translate( decalx, decaly - 3, -0.2 ) ;

// projection
    m_text[0]->draw() ;
    m_text[1]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
