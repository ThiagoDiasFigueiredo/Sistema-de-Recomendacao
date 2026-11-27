from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "solver",
        sources=[
            "solver.cpp",
            "alocar_matriz.cpp",
            "similaridade.cpp",
            "matrizes_esparsas.cpp",
            "recomendacao.cpp",
        ],
        include_dirs=["."],
    ),
]

setup(
    name="solver",
    version="1.0",
    cmdclass={"build_ext": build_ext},
    ext_modules=ext_modules,
)