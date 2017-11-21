# Docker image to build and test Plumed2

# Start from CentOS 6.9
FROM centos:6.9

# Install system packages
RUN yum install --disablerepo=updates --assumeyes \
                gcc-c++ \
                git \
                wget \
    && yum clean all

# Install Miniconda
ENV CONDA_URL https://repo.continuum.io/miniconda
ENV CONDA_INSTALLER Miniconda3-latest-Linux-x86_64.sh
ENV CONDA_PATH /opt/miniconda3
RUN cd tmp &&\
    wget $CONDA_URL/$CONDA_INSTALLER 2> /dev/null &&\
    bash $CONDA_INSTALLER -b -p $CONDA_PATH &&\
    rm $CONDA_INSTALLER
ENV PATH $CONDA_PATH/bin:$PATH

# Install Plumed2 dependencies
#RUN conda install --channel acellera --yes \
#                  openmm cuda-runtime openmm-plumed2 &&\
#    conda clean --all --yes