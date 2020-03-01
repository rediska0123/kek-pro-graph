#ifndef AUTOMATION_H
#define AUTOMATION_H

class Automation {
  public:
	Automation();
	~Automation();
	void add_state(int st);
	void add_edge(int from_st, int to_st, char s);

  private:
};

#endif
