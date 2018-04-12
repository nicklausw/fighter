code()
if pc() < $C08000 {
  seek($C08000)
}

function init {
  sei // no interrupts
  clc; xce // no emulate
  jsl ^initCode // for banking and zero
}

function rtiPointer {
    rti
}
