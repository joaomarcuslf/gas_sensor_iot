float read_gas_senson();

float read_gas_senson() {
  float h = analogRead(A0);
  if (isnan(h))
  {
    return -1.0;
  }
  else
  {
    return (h / 1023 * 100);
  }
}
