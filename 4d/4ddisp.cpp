int main() {
  
  return 0;


bool getvalue(int cr, int ci, int zr, int zi) {
  index = cr*250*250*250+ci*250*250+zr*250+zi;
  return (julia[index>>3]>>(index & 7)) & 1;

}
