# *****************************************************************************
#
# Copyright (c) 2019, the Perspective Authors.
#
# This file is part of the Perspective library, distributed under the terms of
# the Apache License 2.0.  The full license can be found in the LICENSE file.
#

FROM python:3.7

RUN apt-get update
RUN apt-get -y install apt-transport-https libtbb-dev cmake rapidjson-dev sudo
RUN curl -sL https://deb.nodesource.com/setup_8.x | sudo bash -
RUN apt-get -y install npm
RUN apt-get -y remove python3

RUN python3.7 -m pip install numpy pandas

RUN python3 -m pip install codecov nose2 mock flake8 pytest pytest-cov
RUN python3 -m pip install 'numpy>=1.13.1' 'pandas>=0.22.0' 'pyarrow==0.14.1'

RUN npm install --global yarn
RUN yarn --version