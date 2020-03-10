/*=========================================================================
 *
 *  Copyright David Doria 2011 daviddoria@gmail.com
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

/*=========================================================================
  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/

#include "vtkOBJWriter.h"

#include "vtkActorCollection.h"
#include "vtkAssemblyNode.h"
#include "vtkAssemblyPath.h"
#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkGeometryFilter.h"
#include "vtkMapper.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkProperty.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"
#include "vtkTransform.h"
#include "vtkSmartPointer.h"
#include "vtkInformationVector.h"
#include "vtkInformation.h"

vtkStandardNewMacro(vtkOBJWriter)

vtkOBJWriter::vtkOBJWriter()
{
  this->FileName = NULL;

  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(0);
}

vtkOBJWriter::~vtkOBJWriter()
{
  if ( this->FileName )
    {
    delete [] this->FileName;
    }
}

int vtkOBJWriter::RequestData(vtkInformation *vtkNotUsed(request),
                               vtkInformationVector **inputVector,
                               vtkInformationVector *vtkNotUsed(outputVector))
{

  //Get the input
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *pd = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));

  //open the file for writing
  std::ofstream fout(this->FileName);

  //write header
  fout << "# wavefront obj file written by the visualization toolkit" << endl << endl;

  fout << "mtllib NONE" << endl << endl;

  vtkSmartPointer<vtkPointData> pntData;
  vtkSmartPointer<vtkPoints> points;
  vtkSmartPointer<vtkDataArray> tcoords;
  int i, i1, i2, idNext;
  int idStart = 1;
  double p[3];
  vtkCellArray *cells;
  vtkIdType npts = 0;
  vtkIdType *indx = 0;

  // write out the points
  for (i = 0; i < pd->GetNumberOfPoints(); i++)
    {
    pd->GetPoint(i, p);
    fout << "v " << p[0] << " " << p[1] << " " << p[2] << endl;
    }

  idNext = idStart + static_cast<int>(pd->GetNumberOfPoints());

  // write out the point data

  vtkSmartPointer<vtkDataArray> normals = pd->GetPointData()->GetNormals();
  if(normals)
    {
      for (i = 0; i < normals->GetNumberOfTuples(); i++)
        {
        normals->GetTuple(i, p);
        fout << "vn " << p[0] << " " << p[1] << " " << p[2] << endl;
        }
    }

  tcoords = pd->GetPointData()->GetTCoords();
  if (tcoords)
    {
    for (i = 0; i < tcoords->GetNumberOfTuples(); i++)
      {
      tcoords->GetTuple(i, p);
      fout << "vt " << p[0] << " " << p[1] << endl;
      }
    }

  // write out a group name and material
  fout << endl << "g grp" << idStart << endl;
  fout << "usemtl mtlNONE" << endl;

  // write out verts if any
  if (pd->GetNumberOfVerts() > 0)
    {
    cells = pd->GetVerts();
    for (cells->InitTraversal(); cells->GetNextCell(npts,indx); )
      {
        fout << "p ";
        for (i = 0; i < npts; i++)
          {
          fout << static_cast<int>(indx[i])+idStart << " ";
          }
        fout << endl;
      }
    }

  // write out lines if any
  if (pd->GetNumberOfLines() > 0)
    {
    cells = pd->GetLines();
    for (cells->InitTraversal(); cells->GetNextCell(npts,indx); )
      {
        fout << "l ";
        if (tcoords)
          {
          for (i = 0; i < npts; i++)
            {
            fout << static_cast<int>(indx[i])+idStart << "/" << static_cast<int>(indx[i]) + idStart << " ";
            }
          }
        else
          {
          for (i = 0; i < npts; i++)
            {
            fout << static_cast<int>(indx[i])+idStart << " ";
            }
          }
        fout << endl;
      }
    }

  // write out polys if any
  if (pd->GetNumberOfPolys() > 0)
    {
    cells = pd->GetPolys();
    for (cells->InitTraversal(); cells->GetNextCell(npts,indx); )
      {
      fout << "f ";
      for (i = 0; i < npts; i++)
        {
        if (normals)
          {
          if (tcoords)
            {
            fout << static_cast<int>(indx[i])+idStart << "/" << static_cast<int>(indx[i])+ idStart << "/" << static_cast<int>(indx[i]) + idStart << " ";
            }
          else
            {
            fout << static_cast<int>(indx[i])+idStart << "//" << static_cast<int>(indx[i]) + idStart << " ";
            }
          }
        else
          {
          if (tcoords)
            {
            fout << static_cast<int>(indx[i])+idStart << "/" << static_cast<int>(indx[i]) + idStart << " ";
            }
          else
            {
            fout << static_cast<int>(indx[i])+idStart << " ";
            }
          }
        }
        fout << endl;
      }
    }

  // write out tstrips if any
  if (pd->GetNumberOfStrips() > 0)
    {
    cells = pd->GetStrips();
    for (cells->InitTraversal(); cells->GetNextCell(npts,indx); )
      {
      for (i = 2; i < npts; i++)
        {
        if (i%2)
        {
            i1 = i - 1;
            i2 = i - 2;
        }
        else
        {
            i1 = i - 1;
            i2 = i - 2;
        }
        if(normals)
          {
          if (tcoords)
            {
            fout << "f " << static_cast<int>(indx[i1]) + idStart << "/" << static_cast<int>(indx[i1]) + idStart << "/" <<  static_cast<int>(indx[i1]) + idStart << " ";
            fout << static_cast<int>(indx[i2])+ idStart << "/" << static_cast<int>(indx[i2]) + idStart << "/" << static_cast<int>(indx[i2]) + idStart << " ";
             fout << static_cast<int>(indx[i]) + idStart << "/" << static_cast<int>(indx[i]) + idStart << "/" << static_cast<int>(indx[i]) + idStart << endl;
            }
          else
            {
            fout << "f " << static_cast<int>(indx[i1]) + idStart << "//" << static_cast<int>(indx[i1]) + idStart << " ";
            fout << static_cast<int>(indx[i2]) + idStart << "//" << static_cast<int>(indx[i2]) + idStart << " ";
            fout << static_cast<int>(indx[i]) + idStart << "//" << static_cast<int>(indx[i]) + idStart << endl;
            }
          }
        else
          {
          if (tcoords)
            {
            fout << "f " << static_cast<int>(indx[i1]) + idStart << "/" << static_cast<int>(indx[i1]) + idStart << " ";
            fout << static_cast<int>(indx[i2]) + idStart << "/" << static_cast<int>(indx[i2]) + idStart << " ";
            fout << static_cast<int>(indx[i]) + idStart << "/" << static_cast<int>(indx[i]) + idStart << endl;
            }
          else
            {
            fout << "f " << static_cast<int>(indx[i1]) + idStart << " " << static_cast<int>(indx[i2]) + idStart << " " << static_cast<int>(indx[i]) + idStart << endl;
            }
          }
        }
      }
    }

  idStart = idNext;

  return 1;
}

void vtkOBJWriter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  if (this->FileName)
    {
    os << indent << "FileName: " << this->FileName << "\n";
    }
  else
    {
    os << indent << "FileName: (null)\n";
    }
}
