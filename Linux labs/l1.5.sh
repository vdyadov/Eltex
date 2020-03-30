#!/bin/bash

ls -l | sed s/^[-rwx]*/Файл:/ | sed s/^d[-rwx]*/Файл:/
