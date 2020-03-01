#include "parse_regex.h"
#include <iostream>
#include <algorithm>

using namespace std;

#define PANIC(s, id) { 										\
	throw invalid_argument("Parse error: string " + s +		\
		"\n\tat index " + to_string(id));					\
}

TreeNode *parse_regex(string regex) {
	vector <int> pair_bracket((int)regex.length(), -1);
	vector <int> open_brackets;
	for (int i = 0; i < (int)regex.length(); i++)
		if (i == 0 || regex[i - 1] != '\\') {
			if (regex[i] == '(')
				open_brackets.push_back(i);
			if (regex[i] == ')') {
				if (open_brackets.empty())
					PANIC(regex, i);
				pair_bracket[open_brackets.back()] = i;
				pair_bracket[i] = open_brackets.back();
				open_brackets.pop_back();
			}
		}
	if (!open_brackets.empty())
		PANIC(regex, (int)regex.length());
	
	const vector <char> op = {'(', ')', '|', '*'};
	
	vector <pair <TreeNode*, int> > st;
	vector <pair <char, int> > operations_waiting;
	
	auto concatenate = [&operations_waiting, &st]() mutable {
		int begin_index = operations_waiting.empty()? 0 :
			operations_waiting.back().second;
		while (st.size() >= 2 && st[(int)st.size() - 2].second >= begin_index) {
			TreeNode *y = st.back().first;
			st.pop_back();
			TreeNode *x = st.back().first;
			int id = st.back().second;
			st.pop_back();
			st.push_back(make_pair(
				new TreeNode(' ', CONCAT, {x, y}),
				id));
		}
	};
	
	for (int i = 0; i < (int)regex.length(); i++) {
		switch (regex[i]) {
			case '\\': {
				if (i + 1 < (int)regex.length()) {
					if (find(op.begin(), op.end(), regex[i + 1]) != op.end()) {
						st.push_back(make_pair(
							new TreeNode(regex[i + 1], SYMBOL, {}),
							i + 1));
						i++;
					} else
						PANIC(regex, i);
				} else
					PANIC(regex, i);
			} break;
			case '(': {
				TreeNode *x = parse_regex(regex.substr(i + 1, pair_bracket[i] - i - 1));
				st.push_back(make_pair(x, i));
				i = pair_bracket[i];
			} break;
			case ')': {} break;
			case '*': {
				if (st.empty())
					PANIC(regex, i);
				TreeNode *tn = new TreeNode('*', STAR, {st.back().first});
				st.pop_back();
				st.push_back(make_pair(tn, i));
			} break;
			case '|': {
				concatenate();
				operations_waiting.push_back(make_pair('|', i));
			} break;
			default: {
				st.push_back(make_pair(
					new TreeNode(regex[i], SYMBOL, {}),
					i));
			} break;
		}
	}
	concatenate();
	
	TreeNode *root = st.empty() ? nullptr : st.back().first;
	for (int i = (int)st.size() - 2; i >= 0; i--) {
		if (operations_waiting.empty())
			PANIC(regex, st[i].second);
		if (st[i].second > operations_waiting.back().second ||
			st[i + 1].second < operations_waiting.back().second)
			PANIC(regex, st[i].second);
		root = new TreeNode('|', UNION, {st[i].first, root});
		operations_waiting.pop_back();
	}
	
	if (!operations_waiting.empty())
		PANIC(regex, operations_waiting.back().second);
	
	return root;
}
