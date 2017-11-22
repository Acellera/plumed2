# Docker image to build and test Plumed2

# Start from CentOS 6.9
FROM centos:6.9

# Install system packages
RUN yum update --assumeyes &&\
    yum install --assumeyes \
                gcc-c++ \
                git \
                vim-enhanced \
                wget &&\
    yum clean all

# Install Miniconda
ENV CONDA_URL https://repo.continuum.io/miniconda
ENV CONDA_INSTALLER Miniconda3-latest-Linux-x86_64.sh
ENV CONDA_PATH /opt/miniconda3
RUN cd tmp &&\
    wget $CONDA_URL/$CONDA_INSTALLER 2> /dev/null &&\
    bash $CONDA_INSTALLER -b -p $CONDA_PATH &&\
    rm $CONDA_INSTALLER
ENV PATH $CONDA_PATH/bin:$PATH

# Install conda packages
RUN conda install --yes\
                  conda-build &&\
    conda clean --all --yes

WORKDIR /tmp/conda-plumed2.git
CMD sleep 3600