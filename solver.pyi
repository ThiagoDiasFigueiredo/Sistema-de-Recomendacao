"""
Integracao Python-C++ do Sistema de Recomendacao
"""
from __future__ import annotations
import collections.abc
import typing
__all__: list[str] = ['recomendacao', 'similaridade']
def recomendacao(arg0: collections.abc.Sequence[collections.abc.Sequence[typing.SupportsInt | typing.SupportsIndex]], arg1: typing.SupportsInt | typing.SupportsIndex, arg2: typing.SupportsInt | typing.SupportsIndex, arg3: typing.SupportsInt | typing.SupportsIndex, arg4: typing.SupportsInt | typing.SupportsIndex) -> list[tuple[int, float]]:
    ...
def similaridade(arg0: collections.abc.Sequence[collections.abc.Sequence[typing.SupportsInt | typing.SupportsIndex]], arg1: typing.SupportsInt | typing.SupportsIndex, arg2: typing.SupportsInt | typing.SupportsIndex, arg3: typing.SupportsInt | typing.SupportsIndex, arg4: typing.SupportsInt | typing.SupportsIndex) -> tuple[int, float, float, float, float, float]:
    ...
