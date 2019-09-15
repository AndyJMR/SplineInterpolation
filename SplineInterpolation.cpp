#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>
#include <vtkSphereSource.h>
#include <vtkGlyph3DMapper.h>

int main(int, char *[])
{
	double p0[3] = { 1.0, 0.0, 0.0 };
	double p1[3] = { 3.0, 2.0, 0.0 };
	double p2[3] = { 5.0, 0.0, 0.0 };

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(p0);
	points->InsertNextPoint(p1);
	points->InsertNextPoint(p2);

	vtkSmartPointer<vtkParametricSpline> spline =
		vtkSmartPointer<vtkParametricSpline>::New();
	spline->SetPoints(points);

	// Method1
#if (0)
	vtkSmartPointer<vtkPoints> outPoints =
		vtkSmartPointer<vtkPoints>::New();
	int pointsCnt = 11;
	double step = 1.0 / (pointsCnt-1);
	for (double i = 0; i <= 1; i = i + step)
	{
		double u[] = {i, 0, 0};
		double p[3];
		spline->Evaluate(u, p, NULL);
		outPoints->InsertNextPoint(p);
	}
#else
	// Method2
#define DIS(p1, p2) (sqrt((p1[0]-p2[0])*(p1[0]-p2[0]) + (p1[1]-p2[1])*(p1[1]-p2[1]) + (p1[2]-p2[2])*(p1[2]-p2[2])))
	vtkSmartPointer<vtkPoints> outPoints =
		vtkSmartPointer<vtkPoints>::New();

	double totalLen = DIS(p0, p1) + DIS(p1, p2);
	double start = 0;
	double step = .3;

	while (start <= totalLen)
	{
		double u[] = { start/totalLen, 0, 0 };
		double p[3];
		spline->Evaluate(u, p, NULL);
		outPoints->InsertNextPoint(p);
		start += step;
	}
#endif

	vtkSmartPointer<vtkParametricSpline> outSpline =
		vtkSmartPointer<vtkParametricSpline>::New();
	outSpline->SetPoints(outPoints);

	vtkSmartPointer<vtkParametricFunctionSource> functionSource =
		vtkSmartPointer<vtkParametricFunctionSource>::New();
	functionSource->SetParametricFunction(outSpline);
	functionSource->Update();

	vtkSmartPointer<vtkPolyDataMapper> splineMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	splineMapper->SetInputConnection(functionSource->GetOutputPort());

	vtkSmartPointer<vtkActor> splineActor =
		vtkSmartPointer<vtkActor>::New();
	splineActor->SetMapper(splineMapper);

	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetPhiResolution(21);
	sphereSource->SetThetaResolution(21);
	sphereSource->SetRadius(.05);

	vtkSmartPointer<vtkPolyData> splinePointsData =
		vtkSmartPointer<vtkPolyData>::New();
	splinePointsData->SetPoints(outPoints);

	vtkSmartPointer<vtkGlyph3DMapper> splinePointsMapper =
		vtkSmartPointer<vtkGlyph3DMapper>::New();
	splinePointsMapper->SetInputData(splinePointsData);
	splinePointsMapper->SetSourceConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> pointsActor =
		vtkSmartPointer<vtkActor>::New();
	pointsActor->SetMapper(splinePointsMapper);
	pointsActor->GetProperty()->SetColor(1, 0, 0);

	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(600, 600);
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->AddActor(splineActor);
	renderer->AddActor(pointsActor);

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}