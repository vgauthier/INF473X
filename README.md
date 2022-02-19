# INF473X
Cybersecurity - The Hacking Xperience

  build:
    docker:
      - image: cimg/base:2021.04
    steps:
      - checkout
      - run: make gcc