// To Compile
#include "server.h"

void print_array(char *values[]);

int main(int argc, char** argv) {
  char *add[3] = {"+", "4", "5"};
  char *sub[3] = {"-", "4", "5"};
  char *mult[3] = {"*", "4", "5"};
  char *div1[3] = {"/", "1", "7"};
  char *div2[3] = {"/", "6", "0"};
  char *power[3] = {"^", "2", "10"};
  char *sqrt1[2] = {"sqrt", "1024"};
  char *sqrt2[2] = {"sqrt", "-1024"};

  char *response[2] = {"", ""};

  comp_protocol(add, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(sub, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(mult, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(div1, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(div2, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(power, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(sqrt1, response);
  print_array(response);

  printf("\n========\n");

  comp_protocol(sqrt2, response);
  print_array(response);
  return 0;
}

void comp_protocol(char *values[], char *response[]) {
  char *sqrt_detect = strstr(values[0], "sqrt");
  char *operator = values[0];
  int integer1 = atoi(values[1]);
  int integer2;
  char result_str[9999];
  int result;
  double double_result;

  response[0] = "OK";

  if(sqrt_detect) {
    printf("sqrt(%d)\n", integer1);

    if(integer1 < 0) {
      response[0] = "Negative Square Root";
      double_result = -9999.0;
    }
   else{
     double_result = sqrt(integer1);
   }

   sprintf(result_str, "%f", double_result);
   printf("Result: %f\n", double_result);
  }
  else {
   integer2 = atoi(values[2]);
   printf("%d %s %d\n", integer1, operator, integer2);
   char single_operator = (char)*operator;

   switch(single_operator) {
     case '+':
      result = integer1 + integer2;
      break;
     case '-':
      result = integer1 - integer2;
      break;
     case '*':
      result = integer1 * integer2;
      break;
     case '/':
      if(integer2 == 0){
        response[0] = "Division by Zero";
        double_result = -9999.0;
      }
      else{
        double_result = (double)integer1 / integer2;
      }
      break;
     case '^':
      result = pow(integer1, integer2);
      break;
   }

   if(single_operator == '/') {
     sprintf(result_str, "%f", double_result);
     printf("Result: %f\n", double_result);
   }
   else{
     sprintf(result_str, "%d", result);
     printf("Result: %d\n", result);
   }

   response[1] = result_str;
  }
}

void print_array(char *values[]) {
  for(int i=0; i < 2; i++) {
    printf("%s\n", values[i]);
  }
}
