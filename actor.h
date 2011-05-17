class Actor {
private:
	Point pos;
	Point old_pos;
public:

};

class Ball : public Actor {
private:
	Vector speed;
};

class Player : public Actor {
};
