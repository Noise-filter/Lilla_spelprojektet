#ifndef INPUT_H
#define INPUT_H

#include "stdafx.h"

class Input
{
	private:
		D3DXVECTOR3 prevMousePos;
		MouseState* mState;

	public:
		Input();
		~Input(){};

		float mousePitch();
		float mouseRotateY();
		bool checkKeyDown(int vkCode);
		bool checkMovement(char input) const;

		void updateMs(MouseState* mState);
		MouseState* getMs();	
		void resetBtnState();
};

#endif