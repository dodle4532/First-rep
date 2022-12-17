#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lab3.h"

typedef enum
{
    E_INVALID = 0,
    E_ADD_GROUP,
    E_ADD_STUDENT,
    E_DEL_GROUP,
    E_DEL_STUDENT,
    E_PRINT_UNI,
    E_PRINT_GROUP,
    E_PRINT_STUDENT,
    E_END
} EAction;

const char *Enum2Str(EAction eAct)
{
    switch (eAct)
    {
        case E_INVALID:
            return "- Невалидная команда";
        case E_ADD_GROUP:
            return "- Добавить группу";
        case E_ADD_STUDENT:
            return "- Добавить студента";
        case E_DEL_GROUP:
            return "- Удалить группу";
        case E_DEL_STUDENT:
            return "- Удалить студента";
        case E_PRINT_UNI:
            return "- Напечатать информацию об университете";
        case E_PRINT_GROUP:
            return "- Напечатать информацию о группе";
        case E_PRINT_STUDENT:
            return "- Напечатать информацию о студенте";
        case E_END:
            return "- Окончание работы";
        default:
            return "? - Невалидная команда";
    }
}

int main(int argc, char* argv[])
{
	char inputFile[SIZE];
	char outputFile[SIZE];
	if (argc == 2) {
		printf("Введите путь выходного файла\n");
		scanf("%s", outputFile);
		strcpy(inputFile, argv[1]);	
	}
	else if (argc != 2 && argc != 3) {
		printf("Введите путь входного и выходного файл\n");
		scanf("%s", inputFile);
		scanf("%s", outputFile);
	}
	else {
		strcpy(inputFile, argv[1]);	
		strcpy(outputFile, argv[2]);
	}	
	University *univ = initUniversity((const char*)inputFile);	
	EAction eAction = E_INVALID;
	Group group;
    group.students = NULL;
    group.studentsCount = 0;
//	group.students = calloc(1, sizeof(Student));
	Group gr;
    gr.students = NULL;
    gr.studentsCount = 0;
	Student student;
	student.id = 0;
	student.birthYear = 2005;
	char c1[SIZE];
	unsigned ind_1 = 0;
	int ind_2 = 0;
	int ind_3 = 0;
	int ind_4 = 0;
	eAction = 0;
	while (eAction != 8) {
		printf("\n");
		EAction eAction = E_INVALID + 1;
        while (eAction != E_END + 1)
        {
            printf("%d %s\n",eAction, Enum2Str(eAction));
            ++eAction;
        }
		eAction = 0;
		printf("Выберите команду\n");
		scanf("%d", &eAction);			
		printf("\n");
		switch (eAction)
		{
			case E_INVALID:
        		printf("Невалидная команда\n");
				break;
        	case E_ADD_GROUP:
				printf("Введите название группы\n");
				scanf("%s", group.name);
        		addNewGroup(univ, group);
				break;
        	case E_ADD_STUDENT:
				printf("Введите название группы\n");
				scanf("%s", c1);
				printf("Введите фамилию и имя студента\n");
				scanf("%s %s",student.surname, student.name);
				strcpy(student.groupName, c1);
        		addNewStudent(getGroup(univ, c1), student);
				break;
        	case E_DEL_GROUP:
				printf("Введите название группы, которую хотите удалить\n");
				scanf("%s", c1);
        		removeGroup(univ, c1);
				break;
        	case E_DEL_STUDENT:
				printf("Введите id студента, которого хотите удалить\n");
                scanf("%u", &ind_1);
                removeStudent(univ, ind_1);
				break;
        	case E_PRINT_UNI:
				printUniversity(univ);
				break;
        	case E_PRINT_GROUP:
				printf("Введите номер группы\n");
				scanf("%d", &ind_2);
				if (ind_2 >= 1 && ind_2 <= univ->groupsCount) {
					printGroup(univ->groups[ind_2 - 1]);
					break;
				}
				break;
        	case E_PRINT_STUDENT:
				printf("Введите номер группы и номер студента в этой группе\n");
                scanf("%d %d", &ind_3, &ind_4);
                if (ind_3 >= 1 && ind_3 <= univ->groupsCount) {
					if(ind_4 >= 1 && ind_4 <= univ->groups[ind_3 - 1].studentsCount) {
						printStudent(univ->groups[ind_3 - 1].students[ind_4 - 1]);
						break;
                    }
				}	
				break;
        	case E_END:
        		printf("Окончание работы\n");
				saveToFile(outputFile, univ);
				freeUniversity(univ);
       	 		return 0;

        	default:
        		printf("Невалидная команда\n");
				break;
				
			printf("\n");
			}		
		}
	saveToFile(outputFile, univ);
    freeUniversity(univ);
	return 0;
}
