#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "lab3.h"

unsigned long g_Id = 1;

void printStudent(const Student student) {
	if (student.id == 0) {
		return;
	}
	printf("    %u\n", student.id);
	printf("        %s %s\n", student.surname, student.name);
	printf("        %u\n", student.birthYear);
}

University* initUniversity(const char *fileName) {
    University* un;
	if (fileName == NULL) {
		return un;
	}
	if (!(un = (University *)calloc(1, sizeof(University)))) {
		return un;
	}
    FILE *fd = fopen(fileName, "rb");
    if (fd == NULL) {
        return un;
    }
    Student student;
	char gr[SIZE];
	int i = -1;
	int j = 0;
    unsigned max = 0;
    while (fread(&student,sizeof(Student), 1, fd)) {
        if (student.id > max ) {
	        g_Id = student.id + 1;
        }
	    if (strcmp(gr, student.groupName) != 0) {
		    ++i;
		    j = 0;
		    ++un->groupsCount;
		    if (un->groupsCount == 1) {
			    if (!(un->groups = calloc(1, sizeof(Group)))) {
				    return un;
			    }
		    }
		    else {
			    if (!(un->groups = realloc(un->groups, 
                      un->groupsCount * sizeof(Group)))) {
			        return un;
			    }
		    } 
	    }
		strcpy(un->groups[i].name, student.groupName); 
		++un->groups[i].studentsCount;
		if (un->groups[i].studentsCount == 1) {
			if (!(un->groups[i].students = calloc(1, sizeof(Student)))) {
				return un;
			}
		}
		else {
			if (!(un->groups[i].students = realloc(un->groups[i].students,
                              un->groups[i].studentsCount *sizeof(Student)))) {
				return un;
			}
		} 
		un->groups[i].students[j] = student;
		strcpy(gr, student.groupName);
		++j;
        }
	fclose(fd);	
    return un;
}

void printUniversity(const University *university) {
	if (university == NULL) {
		return;
	}
	if (university->groupsCount == 0){
		printf("Все студенты из вуза отчислены!!!\n");
		return;
	}
	for (int i = 0; i < university->groupsCount; ++i) {
		if (university->groups[i].studentsCount == 0) {
			continue;
		}
		printf("-------------------------------------");
		printf("------------------------------------------\n");
		printGroup(university->groups[i]);
	}
}

void printGroup(const Group group) {
	if (group.studentsCount == 0) {
        printf("Вся группа отчислена!!!\n");
        return;
    }
	printf("%s\n", group.name);
	for (int j = 0; j < group.studentsCount; ++j) {
		printStudent(group.students[j]);
    }	
}

bool addNewGroup(University *university, const Group group) {
	if (university == NULL) {
		return false;
	}
	university->groupsCount += 1;
    Group *t;
	if (!(t = realloc(university->groups, 
          university->groupsCount  * sizeof(Group)))) {
	    return false;
	}
    university->groups = t;
	university->groups[university->groupsCount - 1] = group;
	strcpy(university->groups[university->groupsCount - 1].name, group.name);
	return true;
}

char *_strtoupper(char *str) {
    if (str == NULL) {
                return NULL;
        }
    for (int i = 0; i < strlen(str); ++i) {
                if (str[i] > 96 && str[i] < 123) {
                        str[i] -=32;
                }
        }
        return str;

}

int strcm(char *s, char *st) {
    char cs[SIZE];
    char cst[SIZE];
    strcpy(cs, s);
    strcpy(cst, st);
    return strcmp(_strtoupper(cs), _strtoupper(cst));
}

bool addNewStudent(Group *group, Student student) {
	if (group == NULL) {
		return false;
	}
	int ind = 0;
	int fl = 0;
	if (student.id == 0) {
		student.id = g_Id;
		++g_Id;
	}
	++group->studentsCount;
    Student *t;
	if (!(t = realloc(group->students, group->studentsCount * sizeof(Student)))) {
		return false;
	}
    group->students = t;
	group->students[group->studentsCount - 1] = student;
	Student tmp;
	for (int i = 0; i < group->studentsCount - 1; ++i) {
		for (int j = 0; j < group->studentsCount - 1 - i; ++j) { 
			if (strcm(group->students[j].surname, group->students[j + 1].surname) > 0) {
				tmp = group->students[j];
				group->students[j] = group->students[j + 1];
				group->students[j + 1] = tmp; 
			}
		}
	} 
	return true;
}

void freeUniversity(University *university) {
	if (university == NULL) {
		return;
	}
	for (int i = 0; i < university->groupsCount ; ++i) {
		if (university->groups[i].studentsCount == 0) {
			continue;
		}
		free(university->groups[i].students);
		university->groups[i].studentsCount = 0;
	}
	free(university->groups);
    free(university);
}

bool removeGroup(University *university, const char *name) {
	if (university == NULL) {
		return NULL;
	}
	int fl = 0;
	for (int i = 0; i < university->groupsCount; ++i) {
		fl = 0;
		if ((strlen(university->groups[i].name)) != strlen(name)) {
			continue;
		}
		for (int j = 0; j < strlen(university->groups[i].name) - 1; ++j) {
			if (name[j] != university->groups[i].name[j]) {
				fl = 1;
				break;
			}
		}
		if (fl == 0) {
			free(university->groups[i].students);
			strcpy(university->groups[i].name, "");
			university->groups[i].studentsCount = 0;
			for (int k = i; k < university->groupsCount; ++k) {
				university->groups[k] = university->groups[k + 1];
			}
			university->groupsCount -= 1;
			university->groups = realloc(university->groups,
			university->groupsCount * sizeof(Group)); 	 
			return true;
		}
	}
	return false;
}

bool removeStudent(University *university, const unsigned long id) {
	if (university == NULL) {
		return NULL;
	}
	int i = 0;
	int j = 0;
	int fl = 0;
	while (i != university->groupsCount) {
		j = 0;
		if (fl == 1){
			break;
		}
		while (j != university->groups[i].studentsCount) {
			if (id == university->groups[i].students[j].id) {
				university->groups[i].students[j].id = 0;
				strcpy(university->groups[i].students[j].name, "");
				strcpy(university->groups[i].students[j].surname, "");
				university->groups[i].students[j].birthYear = 0;	
				fl = 1;
				for (int k = j; k < university->groups[i].studentsCount; ++k) {
					university->groups[i].students[k] = 
					university->groups[i].students[k + 1];	
				}
				university->groups[i].studentsCount -= 1;
				university->groups[i].students = realloc(university->groups[i].students,
				university->groups[i].studentsCount * sizeof(Student));
				return true;
			}
			++j;
		}
		++i;
	}
	return false;
}

Group* getGroup(const University *university, const char *name) {
	if (university == NULL) {
		return NULL;
	}
	int fl = 0;
        for (int i = 0; i < university->groupsCount; ++i) {
                fl = 0;
                if ((strlen(university->groups[i].name)) != strlen(name)) {
                    continue;
                }
                for (int j = 0; j < strlen(university->groups[i].name) - 1; ++j) {
                    if (name[j] != university->groups[i].name[j]) {
                        fl = 1;
                        break;
                        }
                }
                if (fl == 0) {
                    return &university->groups[i];
                }
        }
        return NULL;
	
}



Student* getStudent(const University *university, const unsigned long id) {
	if (university == NULL) {
		return NULL;
	}
	int i = 0;
        int j = 0;
        int fl = 0;
        while (i != university->groupsCount) {
                j = 0;
                if (fl == 1) {
                        break;
                }
                while (j != university->groups[i].studentsCount) {
                        if (id == university->groups[i].students[j].id) {
                                return &university->groups[i].students[j];
                        }
                        ++j;
                }
                ++i;
        }
        return NULL;
}

bool saveToFile(const char *fileName, const University *university) {
	if (university == NULL) {
		return false;
	}
	if (fileName == NULL) {
		return false;
	}
	FILE *fd = fopen(fileName, "wb+");
	if (fd == NULL) {
		return false;
	}
	for (int i = 0; i < university->groupsCount; ++i) {
		for (int j = 0; j < university->groups[i].studentsCount; ++j) {
			fwrite(&university->groups[i].students[j], sizeof(Student), 1, fd);
		}
	}
	fclose(fd);
	return true;
}

