/*  ---------------------------------------------------------------------------
 *  filename    :   glamtext.cpp
 *  description :   IMPLEMENTATION de la classe GLamText
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

#include "glamtext.h"
#include "glamsolid.h"
#include <QFontDatabase>
#include <QPainterPath>

// constructeur 1 (défaut)

/*! Constructeur par défaut. */

GLamText::GLamText(const QString& text )
	: GLamGroup()
	, m_text( text )
	, m_thickness( 0 )
    , m_materialChanged( false )
{
	setClassName("GLamText" ) ;
	loadDefaultFont() ;
	buildCharAsSolid() ;
}

/*! Constructeur.
 *  Fabrique une chaine de caractères 3D avec la fonte spécifiée en argument. */

GLamText::GLamText(const QString& text, const QFont& font )
	: GLamGroup()
	, m_text( text )
	, m_font( font )
	, m_thickness( 0 )
    , m_materialChanged( false )
{
	setClassName("GLamText" ) ;
	buildCharAsSolid() ;
}

/*! Constructeur de copie. */

GLamText::GLamText(const GLamText& obj )
	: GLamGroup()
{
	*this = obj ;
}

/*! destructeur. */

GLamText::~GLamText()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamText& GLamText::operator =(const GLamText& obj )
{
	*(GLamGroup*)this = obj ;
	m_text = obj.m_text ;
	m_font = obj.m_font ;
	m_thickness = obj.m_thickness ;
    m_materialChanged = obj.m_materialChanged ;
	buildCharAsSolid() ;
    return *this ;
}

// modificateurs

void GLamText::setText(const QString& text )
{
	m_text = text ;
	buildCharAsSolid() ;
}

void GLamText::setFont(const QFont &font )
{
	m_font = font ;
	buildCharAsSolid() ;
}

/*! Propriété @ref text_font "font". Chargement direct d'un fichier au
 *  format Police TrueType (ttf), TrueType Collection (ttc),
 *  data fork TrueType (dfont) ou Font Suitcase. */

void GLamText::setFontByPath(const QString& fontPath )
{
    int id = QFontDatabase::addApplicationFont( fontPath ) ;
    if ( id == -1 ) return ;
    QString family = QFontDatabase::applicationFontFamilies(id).at(0) ;
    m_font = QFont(family, fontSize() ) ;
    buildCharAsSolid() ;
}

void GLamText::setFontSize(qreal size )
{
	m_font.setPointSizeF( size ) ;
	buildCharAsSolid() ;
}

void GLamText::setThickness(qreal thickness )
{
	m_thickness = thickness ;
	buildCharAsSolid() ;
}

// surcharges GLamGroup

void GLamText::setMaterial(GLamMaterial material )
{
    for ( int i = 0 ; i < m_iObj.count() ; ++i ) {
        ((GLamSolid*)(this->object(i)))->setMaterial( material ) ;
    }
    m_material = material ;
}

GLamMaterial& GLamText::material()
{
    m_materialChanged = true ;
    return m_material ;
}

void GLamText::draw(GLamObject* parent )
{
    if ( m_materialChanged )     { setMaterial( m_material ) ; m_materialChanged = false ; }
    GLamGroup::draw( parent ) ;
}

/*! Méthode de concaténation de texte. Le texte ajouté prend les
 *  caractéristiques de l'objet courant (dimensions et fonte) mais conserve son
 *  matériau propre.
 *  @n La couleur du texte ajoutée peut aussi être directement indiquée par
 *  l'argument @c simpleColor.
 */

void GLamText::concat(GLamText& text, QColor simpleColor )
{
    m_text += text.text() ;
    m_concatWidth += fontSize() / 10 ;  // space between substrings
    text.setFont( this->font() ) ;
    text.setThickness( this->thickness() ) ;
    text.defaultMatrix().setToIdentity() ;
    text.defaultMatrix().translate( m_concatWidth, 0 ) ;
    m_concatWidth += text.width() ;

    if ( simpleColor.isValid() )    text.material().setColor( simpleColor ) ;

    int iObj = this->addObject( &text ) ;
    ((GLamText*)(this->object(iObj)))->setMaterial( text.material() ) ;
}

// [private] chargement police par défaut (ressource Arial.ttf)

void GLamText::loadDefaultFont()
{
	int id = QFontDatabase::addApplicationFont(":/res/Arial.ttf") ;
	QString family = QFontDatabase::applicationFontFamilies(id).at(0) ;
	m_font = QFont(family, 3 ) ;
}

// [private] construction des solides ajourés

void GLamText::buildCharAsSolid()
{
	// génération des polygones
	QPainterPath path ;
	path.addText(0, 0 , m_font, m_text ) ;
    m_rect = path.boundingRect() ;
    QList<QPolygonF> poly = path.toSubpathPolygons() ;

	QList<QPointF>	shape ;
	GLamSolid*		contour ;
	GLamProfile*	profile ;

	// si épaisseur requise...
	if ( m_thickness ) {
		// profil légèrement bondé
		float e = m_thickness / 10.0 ;
		QList<QPointF>	pts ;
		pts << QPointF(0, 0 ) ;
		pts << QPointF(e, e ) ;
		pts << QPointF(e, m_thickness - e ) ;
		pts << QPointF(0, m_thickness ) ;
		profile = new GLamProfile(pts, 4 ) ;
	}

	this->removeAllObjects() ;
    m_iObj.clear() ;

	int iObj = -1 ;	// indice objet dans groupement
	for ( int i = 0 ; i < poly.size() ; ++i ) {
		shape.clear() ;
		for ( int p = 0 ; p < poly.at(i).size() ; ++p ) {
			shape << QPointF(poly[i][p].rx(), -poly[i][p].ry() ) ;
		}
        shape = GLam2D::invert(shape) ;
		// contour d'un glyphe
        if ( !GLam2D::isClockWise( shape ) ) {
            contour = new GLamSolid(shape, m_thickness ) ;
			if ( m_thickness )	contour->setProfile( profile ) ;
			iObj = this->addObject( contour ) ;
            m_iObj << iObj ;
		}
		// ou contour d'un trou
		else {
            GLamHole hole(GLam2D::invert(shape), m_thickness ) ;
			if ( m_thickness )	hole.setProfile( profile ) ;
			((GLamSolid*)(this->object(iObj)))->addHole( &hole ) ;
		}
	}
    m_concatWidth = m_rect.width() ;
}
