from collections.abc import Generator
import numpy as np
from enum import Enum, auto
from typing import Literal, TypeVar
from dataclasses import dataclass
from pathlib import Path

from ..api import AUExtractor  # type: ignore
from ..common import PredictionMode


@dataclass(frozen=True)
class Result:
    action_units: np.ndarray[Literal[2], np.dtype[np.uint8]]
    presences: np.ndarray[Literal[2], np.dtype[np.bool_]]
    intensities: np.ndarray[Literal[2], np.dtype[np.float32]]


def time(step: float) -> Generator[float, None, None]:
    current = 0.0

    while True:
        yield current
        current += step


# skip typing - ndarray dtype is an absolute garbage
def as_padded_and_imputed_with_zeros_array(data: list[list | None], dtype) -> np.ndarray:
    n_rows = len(data)
    n_columns: int = len(max(filter(None, data), key=len, default=[]))

    array = np.zeros((n_rows, n_columns), dtype=dtype)

    for i, row in enumerate(data):
        if row is None:
            continue

        np.copyto(array[i, len(row):], row)

    return array


def convert(au_label: str) -> int:
    return int(''.join(filter(str.isdigit, au_label)) or '0')


class Extractor:
    landmark_mode: PredictionMode
    au_mode: PredictionMode
    wild: bool
    multiple_views: bool
    limit_angles: bool
    optimization_iterations: int | None
    regularization_factor: float | None
    weight_factor: float | None

    fps: int

    models: Path

    __model: AUExtractor
    __time: Generator[float, None, None]
    __time_step: float = 1.0 / 60.0

    def __init__(
            self,
            *,
            landmark_mode: PredictionMode,
            au_mode: PredictionMode,
            fps: int = 60,
            wild: bool = False,
            models_directory: str,
            multiple_views: bool = True,
            limit_angles: bool = False,
            optimization_iterations: int | None = None,
            regularization_factor: float | None = None,
            weight_factor: float | None = None
    ) -> None:
        if optimization_iterations is not None:
            assert optimization_iterations > 0, 'Number of optimization iterations must be positive'

        if regularization_factor is not None:
            assert regularization_factor > 0.0, 'Optimization regularization factor must be positive'

        if weight_factor is not None:
            assert weight_factor > 0.0, 'Optimization weight factor must be positive'

        models = Path(models_directory)
        assert models.exists() and models.is_dir(), 'Invalid models directory passed'
        self.models = models

        self.mode = landmark_mode
        self.wild = wild
        self.multiple_views = multiple_views
        self.limit_angles = limit_angles
        self.optimization_iterations = optimization_iterations
        self.regularization_factor = regularization_factor
        self.weight_factor = weight_factor

        self.fps = fps

        self.__model = AUExtractor(
            landmark_mode == Mode.VIDEO,
            au_mode == Mode.VIDEO,
            wild,
            multiple_views,
            limit_angles,
            optimization_iterations,
            regularization_factor,
            weight_factor,
            models_directory
        )

        self.__time = time(1.0 / float(fps))

    def predict(
            self,
            frame: np.ndarray[Literal[3], np.dtype[np.uint8]] | np.ndarray[Literal[4], np.dtype[np.uint8]],
            region: np.ndarray[Literal[1], np.dtype[np.uint32]] | np.ndarray[Literal[2], np.dtype[np.uint32]]
    ) -> Result | None:
        match frame.shape, region.shape:
            case (height, width, n_channels), (n_elements,):
                assert n_elements == 4, 'Wrong region format: expected 4 elements, got {n_elements}'
                assert n_channels == 3, f'Wrong frame format: expected 3-channel RGB image, got {n_channels} channels'

                prediction = self.__model.detect_action_units(
                    frame,
                    next(self.__time),
                    region
                )

                if prediction is None:
                    return None

                return Result(
                    np.array(([
                        convert(unit[0])
                        for unit in prediction
                    ],), dtype=np.uint8),
                    np.array(([
                        unit[1]
                        for unit in prediction
                    ],), dtype=bool),
                    np.array(([
                        unit[2]
                        for unit in prediction
                    ],), dtype=np.float32),
                )

            case (n_frames, height, width, n_channels), (n_regions, n_elements):
                assert n_frames == n_regions, f'Number of frames ({n_frames}) doesn\'t match number of regions ({n_regions})'
                assert n_elements == 4, f'Wrong region format: expected 4 elements, got {n_elements}'
                assert n_channels == 3, f'Wrong frame format: expected 3-channel RGB image, got {n_channels} channels'

                predictions = [
                    self.__model.detect_action_units(frame, timestamp, region)
                    for frame, timestamp, region
                    in zip(
                        frame,
                        self.__time,
                        region
                    )
                ]

                units = [
                    [convert(unit[0]) for unit in prediction]
                    if prediction is not None else None
                    for prediction in predictions
                ]

                presences = [
                    [unit[1] for unit in prediction]
                    if prediction is not None else None
                    for prediction in predictions
                ]

                intensities = [
                    [unit[2] for unit in prediction]
                    if prediction is not None else None
                    for prediction in predictions
                ]

                return Result(
                    as_padded_and_imputed_with_zeros_array(units, np.uint8),
                    as_padded_and_imputed_with_zeros_array(presences, np.bool_),
                    as_padded_and_imputed_with_zeros_array(intensities, np.float32),
                )

            case _:
                raise RuntimeError(
                    f'Wrong shapes of arguments:\n'
                    f'frame.shape: expected ([n,] height, width, 3), got {frame.shape},\n'
                    f'region.shape: expected ([n,] 4), got {region.shape}'
                )