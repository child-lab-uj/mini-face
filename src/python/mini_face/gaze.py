from collections.abc import Generator
from dataclasses import dataclass
from pathlib import Path
from typing import Literal

import numpy as np

from .api import GazeExtractor  # type: ignore
from .mode import PredictionMode

__all__ = ["Extractor", "Result"]


@dataclass(frozen=True)
class Result:
    eyes: np.ndarray[Literal[3], np.dtype[np.float32]]
    directions: np.ndarray[Literal[3], np.dtype[np.float32]]
    angles: np.ndarray[Literal[2], np.dtype[np.float32]]


def time(step: float) -> Generator[float, None, None]:
    current = 0.0

    while True:
        yield current
        current += step


EMPTY_ENTRY = ((0.0, 0.0, 0.0), (0.0, 0.0, 0.0))


class Extractor:
    mode: PredictionMode
    wild: bool
    multiple_views: bool
    limit_angles: bool
    optimization_iterations: int | None
    regularization_factor: float | None
    weight_factor: float | None

    fps: int
    fx: float
    fy: float
    cx: float
    cy: float

    models: Path

    __model: GazeExtractor
    __time: Generator[float, None, None]
    __time_step: float = 1.0 / 60.0

    def __init__(
        self,
        *,
        mode: PredictionMode,
        focal_length: tuple[float, float],
        optical_center: tuple[float, float],
        models_directory: str,
        fps: int = 60,
        wild: bool = False,
        multiple_views: bool = True,
        limit_angles: bool = False,
        optimization_iterations: int | None = None,
        regularization_factor: float | None = None,
        weight_factor: float | None = None,
    ) -> None:
        fx, fy = focal_length
        cx, cy = optical_center

        assert fx > 0.0, "Focal length components must be positive"
        assert fy > 0.0, "Focal length components must be positive"
        assert cx > 0.0, "Optical center coordinates must be positive"
        assert cy > 0.0, "Optical center coordinates must be positive"

        if optimization_iterations is not None:
            assert (
                optimization_iterations > 0
            ), "Number of optimization iterations must be positive"

        if regularization_factor is not None:
            assert (
                regularization_factor > 0.0
            ), "Optimization regularization factor must be positive"

        if weight_factor is not None:
            assert weight_factor > 0.0, "Optimization weight factor must be positive"

        models = Path(models_directory)
        assert models.exists() and models.is_dir(), "Invalid models directory passed"
        self.models = models

        self.mode = mode
        self.wild = wild
        self.multiple_views = multiple_views
        self.limit_angles = limit_angles
        self.optimization_iterations = optimization_iterations
        self.regularization_factor = regularization_factor
        self.weight_factor = weight_factor

        self.fps = fps
        self.fx = fx
        self.fy = fy
        self.cx = cx
        self.cy = cy

        model = GazeExtractor(
            models_directory,
            mode == PredictionMode.VIDEO,
            wild,
            multiple_views,
            limit_angles,
            optimization_iterations,
            regularization_factor,
            weight_factor,
        )

        model.set_camera_calibration(fx, fy, cx, cy)

        self.__model = model
        self.__time = time(1.0 / float(fps))

    def predict(
        self,
        frame: np.ndarray[Literal[3], np.dtype[np.uint8]]
        | np.ndarray[Literal[4], np.dtype[np.uint8]],
        region: np.ndarray[Literal[1], np.dtype[np.uint32]]
        | np.ndarray[Literal[2], np.dtype[np.uint32]],
    ) -> Result | None:
        match frame.shape, region.shape:
            case (_, _, n_channels), (n_elements,):
                assert (
                    n_elements == 4
                ), "Wrong region format: expected 4 elements, got {n_elements}"
                assert (
                    n_channels == 3
                ), f"Wrong frame format: expected 3-channel RGB image, got {n_channels} channels"

                result = self.__model.detect_gaze(
                    frame, next(self.__time), tuple(region)
                )

                if result is None:
                    return None

                return Result(
                    np.array(((result.eye1, result.eye2),), dtype=np.float32),
                    np.array(
                        ((result.direction1, result.direction2),), dtype=np.float32
                    ),
                    result.angle,
                )

            case (n_frames, _, _, n_channels), (n_regions, n_elements):
                assert (
                    n_frames == n_regions
                ), f"Number of frames ({n_frames}) doesn't match number of regions ({n_regions})"
                assert (
                    n_elements == 4
                ), f"Wrong region format: expected 4 elements, got {n_elements}"
                assert (
                    n_channels == 3
                ), f"Wrong frame format: expected 3-channel RGB image, got {n_channels} channels"

                predictions = [
                    self.__model.detect_gaze(frame, timestamp, tuple(region))
                    for frame, timestamp, region in zip(frame, self.__time, region)
                ]

                eyes = np.array(
                    [
                        (prediction.eye1, prediction.eye2)
                        if prediction is not None
                        else EMPTY_ENTRY
                        for prediction in predictions
                    ],
                    dtype=np.float32,
                )

                directions = np.array(
                    [
                        (prediction.direction1, prediction.direction2)
                        if prediction is not None
                        else EMPTY_ENTRY
                        for prediction in predictions
                    ],
                    dtype=np.float32,
                )

                angles = np.array(
                    [
                        prediction.angle if prediction is not None else 0.0
                        for prediction in predictions
                    ],
                    dtype=np.float32,
                )

                return Result(eyes, directions, angles)

            case _:
                raise RuntimeError(
                    f"Wrong shapes of arguments:\n"
                    f"frame.shape: expected ([n,] height, width, 3), got {frame.shape},\n"
                    f"region.shape: expected ([n,] 4), got {region.shape}"
                )
