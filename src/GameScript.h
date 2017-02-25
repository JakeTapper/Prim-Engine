#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class GameObject;

class GameScript {
public:
	GameScript(GameObject* _parent) {
		parent = _parent;
	}

	virtual void run() = 0;

protected:
	GameObject* parent;
};

class SList {
	struct Node {
		Node* last;
		Node* next;
		GameScript* data;
	};

public:
	SList() {
		dummy = new Node();
		dummy->last = 0;
		dummy->next = 0;
		dummy->data = 0;
	}

	void append(GameScript* g) {
		Node* p = dummy;
		while (p->next)
			p = p->next;
		p->next = new Node();
		p->next->last = p;
		p->next->data = g;
		p->next->next = 0;
	}

	void run() {
		Node* p = dummy;
		while (1) {
			if (!p->next) return;
			p = p->next;
			p->data->run();
		}
	}

private:
	Node* dummy;
};