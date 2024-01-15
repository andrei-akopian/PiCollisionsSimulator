const char HELP_MESSAGE[] = "Help / Manual: \n"
"A purely mathematical simulator of elastic collisions that simulates the phenomenon described in this "
"3Blue1Brown YouTube Short https://www.youtube.com/shorts/P11ykXwx4-k\n"

"Description of the problem: A small block is at rest on a plane with a vertical wall on the left. "
"A block N times in mass is sliding from the right to the left with some speed, "
"the maximum number of elastic collisions that will ever happen for a scenario will be PI * sqrt(N) and "
"will thus show the digits of PI if N is 100 to some power.\n"
"Source code can be found here: https://github.com/andrei-akopian/PiCollisionsSimulator\n\n" //TODO add github link here

"Usage instructions (Warning! there is no input validation):\n"  
"-n: smaller_mass * n = larger_mass\n"
"-c: generate x digits of pi\n"
"-sm: smaller_mass mass\n"
"-lm: larger_mass mass (initally negative, left is negative direction)\n"
"-sv: smaller_mass initial velocity\n"
"-lv: larger_mass initial velocity\n"
"-e: display energy (always the same)\n"
"-m: display momentum (changes)\n"
"-v: display velocity\n"
"-p: display picture/diagram (not to scale)\n"
"-h or --help: displays this unhelpful help message\n"
"Usage example `./a.out -n 10 -v -p`\n";

#define PICTURE_ARROW_MULTIPLIER 3
// Actuall Code
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

struct inputs {
  float smaller_mass;
  float larger_mass;
  float velocity_smaller;
  float velocity_larger;
  int bool_display_energy;
  int bool_display_momentum;
  int bool_display_velocity;
  int bool_display_pictures;
  int bool_display_atall;
};

struct inputs parseCLI(int argc, char *argv[]){
  struct inputs ins = {1,1,0,-1,0,0,0,0,0};
  int i=1;
  while (i<argc){
    if (strcmp(argv[i],"-sm")==0){
      ins.smaller_mass = atof(argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"-lm")==0){
      ins.larger_mass = atof(argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"-sv")==0){
      ins.velocity_smaller = atof(argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"-lv")==0){
      ins.velocity_larger = atof(argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"-n")==0) {
      ins.larger_mass = ins.smaller_mass*atof(argv[i+1]);
      i++;
    } else if (strcmp(argv[i],"-c")==0){
      ins.larger_mass = ins.smaller_mass*(pow(10,atof(argv[i+1])));
      i++;
    } else if (strcmp(argv[i],"-e")==0){
      ins.bool_display_energy = 1;
    } else if (strcmp(argv[i],"-m")==0){
      ins.bool_display_momentum = 1;
    } else if (strcmp(argv[i],"-v")==0){
      ins.bool_display_velocity = 1;
    } else if (strcmp(argv[i],"-p")==0){
      ins.bool_display_pictures = 1;
    } else if (strcmp(argv[i],"-h")==0 || strcmp(argv[i],"--help")==0 ){
      printf("%s",HELP_MESSAGE);
      exit(0);
    }
    i++;
  }
  ins.bool_display_atall = ins.bool_display_energy+ins.bool_display_momentum+ins.bool_display_velocity+ins.bool_display_pictures;
  return ins;
};

// Calculations
struct velocity_states {
  float velocity_smaller;
  float velocity_larger;
};

float calculate_center_of_mass_velocity(struct velocity_states state, struct inputs masses){
  float weighted_sum = state.velocity_smaller * masses.smaller_mass + state.velocity_larger * masses.larger_mass;
  float sum = masses.smaller_mass + masses.larger_mass;
  return weighted_sum/sum;
}

struct velocity_states ellastic_collision (struct velocity_states state, struct inputs masses){
  float vc = calculate_center_of_mass_velocity(state, masses);
  struct velocity_states new_state = {2*vc-state.velocity_smaller, 2*vc-state.velocity_larger};
  return new_state;
};

struct velocity_states bounce_off_wall (struct velocity_states state){
  struct velocity_states new_state = {state.velocity_smaller*-1, state.velocity_larger};
  return new_state;
};

// Formatting
void print_velocity_state(struct velocity_states state){
  printf("%3f %3f\n",state.velocity_smaller, state.velocity_larger);
};

 void calculate_energy(struct velocity_states state, struct inputs masses){
  float sk = state.velocity_smaller*state.velocity_smaller*masses.smaller_mass/2;
  float lk = state.velocity_larger*state.velocity_larger*masses.smaller_mass/2;
  printf("Energy: Small mass: %.4f\tBig mass: %.4f\t Total: %f\n",sk,lk,sk+lk);
 };

void calculate_momentum(struct velocity_states state, struct inputs masses){
  float smom = state.velocity_smaller*masses.smaller_mass;
  float lmom = state.velocity_larger*masses.larger_mass;
  printf("Momentum: Small mass: %.4f\tBig mass: %.4f\t Total: %f\n",smom,lmom,smom+lmom);
};

void print_block(float v, float vsum){
  if (v<0){
    printf("<");
  } else {
    printf("#");
  }
  for (int i=0; i<(int)(fabsf(v/vsum)*PICTURE_ARROW_MULTIPLIER); i++){
    printf("-");
  }
  if (v<0){
    printf("#");
  } else if (v>0){
    printf(">");
  }
};

void print_picture(struct velocity_states state){
  float total_v = fabsf(state.velocity_smaller)+fabsf(state.velocity_larger);
  printf("| ");
  print_block(state.velocity_smaller,total_v);
  printf("\t");
  print_block(state.velocity_larger,total_v);
  printf("\n");
  }

void print_state(struct velocity_states state, struct inputs ins, long collision_count){
  if (ins.bool_display_atall){
    printf("Collision: %ld\n",collision_count);
  }
  if (ins.bool_display_velocity || collision_count==0){
    print_velocity_state(state);
  }
  if (ins.bool_display_pictures){
    print_picture(state);
  }
  if (ins.bool_display_momentum){
    calculate_momentum(state,ins);
  }
  if (ins.bool_display_energy){
    calculate_energy(state,ins);
  }
};

void print_inputs(struct inputs ins){
  printf("# Transcribed Input Command\n");
  printf("Masses:\t%.4f \t %.4f\n",ins.smaller_mass,ins.larger_mass);
  printf("Initial Velocities:\t%.4f \t %.4f\n",ins.velocity_smaller,ins.velocity_larger);
  if (ins.bool_display_atall){
    printf("Display options:\n");
    if (ins.bool_display_energy){
      printf("\tEnergy\n");
    }
    if (ins.bool_display_momentum){
      printf("\tMomentum\n");
    }
    if (ins.bool_display_velocity){
      printf("\tVelocity\n");
    }
    if (ins.bool_display_pictures){
      printf("\tPictures\n");
    }
  }
  printf("For help use -h option\n\n");
}

int main (int argc, char *argv[]){
  //setup
  struct inputs ins = parseCLI(argc, argv);
  long c = 0; //colision count, shortcuts and bandaid stuff will break if if overflows
  struct velocity_states s = {ins.velocity_smaller, ins.velocity_larger};
  //initial state print
  print_inputs(ins);
  //simulation
  printf("# Collision States:\n");
  int flipper = 1;
  while (!(s.velocity_smaller>=0 && s.velocity_larger>0 && s.velocity_smaller < s.velocity_larger)){
    print_state(s,ins,c);
    if (flipper) {
      s = ellastic_collision(s,ins);
      flipper =0;
    } else {
      s = bounce_off_wall(s);
      flipper =1;
    }
    c++;
  }
  // final sate print
  printf("# Final State:\n");
  print_state(s,ins,0);
  printf("# Total Collision Count:\n%ld\n",c);
  return 0;
};
