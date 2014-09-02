/*
 * Viewer.h
 *
 *  Created on: Sep 2, 2014
 *      Author: lmasello
 */

#ifndef VIEWER_H_
#define VIEWER_H_

class Viewer{
public:
	Viewer(){};
	virtual ~Viewer(){};
	virtual void reproducirVentana() = 0;
};

#endif /* VIEWER_H_ */
